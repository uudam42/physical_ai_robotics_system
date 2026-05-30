#include <chrono>
#include <exception>
#include <future>
#include <memory>
#include <string>

#include "behaviortree_cpp/bt_factory.h"
#include "pai_task_msgs/msg/task_plan.hpp"
#include "pai_task_msgs/msg/task_status.hpp"
#include "pai_task_msgs/srv/execute_robot_action.hpp"
#include "rclcpp/rclcpp.hpp"

namespace pai_bt_executor_cpp
{

using ExecuteRobotAction = pai_task_msgs::srv::ExecuteRobotAction;

namespace
{
constexpr auto kServiceWaitTimeout = std::chrono::seconds(2);
constexpr auto kServiceResponseTimeout = std::chrono::seconds(5);
}  // namespace

class BtExecutorNode : public rclcpp::Node
{
public:
  BtExecutorNode()
  : Node("bt_executor_node")
  {
    declare_parameter<std::string>(
      "bt_xml_path",
      "bt_xml/pick_and_place.xml");

    task_status_publisher_ =
      create_publisher<pai_task_msgs::msg::TaskStatus>("/task_status", 10);

    robot_action_callback_group_ =
      create_callback_group(rclcpp::CallbackGroupType::Reentrant);
    robot_action_client_ = create_client<ExecuteRobotAction>(
      "/robot_action",
      rmw_qos_profile_services_default,
      robot_action_callback_group_);

    task_plan_subscription_ =
      create_subscription<pai_task_msgs::msg::TaskPlan>(
      "/task_plan",
      10,
      [this](pai_task_msgs::msg::TaskPlan::SharedPtr msg) {
        handle_task_plan(*msg);
      });

    RCLCPP_INFO(get_logger(), "C++ BehaviorTree executor node started");
    RCLCPP_INFO(get_logger(), "Listening on /task_plan and publishing to /task_status");
    RCLCPP_INFO(get_logger(), "Calling robot actions through /robot_action");
  }

private:
  void handle_task_plan(const pai_task_msgs::msg::TaskPlan & task_plan)
  {
    if (task_plan.task_type != "pick_and_place") {
      RCLCPP_WARN(
        get_logger(),
        "Task '%s' has unsupported task_type '%s'; rejecting",
        task_plan.task_id.c_str(),
        task_plan.task_type.c_str());
      publish_status(
        task_plan.task_id,
        "",
        "rejected",
        "Unsupported task type");
      return;
    }

    publish_status(
      task_plan.task_id,
      "",
      "running",
      "Executing pick_and_place behavior tree");

    const auto bt_xml_path = get_parameter("bt_xml_path").as_string();
    RCLCPP_INFO(get_logger(), "Loading behavior tree XML: %s", bt_xml_path.c_str());

    try {
      auto factory = create_factory(task_plan);
      auto tree = factory.createTreeFromFile(bt_xml_path);
      const auto result = tree.tickWhileRunning();

      if (result == BT::NodeStatus::SUCCESS) {
        publish_status(
          task_plan.task_id,
          "",
          "completed",
          "Behavior tree completed successfully");
        RCLCPP_INFO(
          get_logger(),
          "Task '%s' completed successfully",
          task_plan.task_id.c_str());
        return;
      }

      const auto result_text = std::string(BT::toStr(result));
      publish_status(
        task_plan.task_id,
        "",
        "failed",
        "Behavior tree did not return SUCCESS");
      RCLCPP_ERROR(
        get_logger(),
        "Task '%s' failed with behavior tree status %s",
        task_plan.task_id.c_str(),
        result_text.c_str());
    } catch (const std::exception & error) {
      publish_status(
        task_plan.task_id,
        "",
        "failed",
        error.what());
      RCLCPP_ERROR(
        get_logger(),
        "Task '%s' failed while executing behavior tree: %s",
        task_plan.task_id.c_str(),
        error.what());
    }
  }

  BT::BehaviorTreeFactory create_factory(const pai_task_msgs::msg::TaskPlan & task_plan)
  {
    BT::BehaviorTreeFactory factory;
    register_robot_action(factory, "CheckObject", "check_object", task_plan.object_name);
    register_robot_action(factory, "MoveToPreGrasp", "move_to_pre_grasp", task_plan.object_name);
    register_robot_action(factory, "OpenGripper", "open_gripper", task_plan.object_name);
    register_robot_action(factory, "MoveToGrasp", "move_to_grasp", task_plan.object_name);
    register_robot_action(factory, "CloseGripper", "close_gripper", task_plan.object_name);
    register_robot_action(factory, "MoveToTarget", "move_to_target", task_plan.target_location);
    register_robot_action(factory, "ReturnHome", "return_home", "home");
    return factory;
  }

  void register_robot_action(
    BT::BehaviorTreeFactory & factory,
    const std::string & bt_node_name,
    const std::string & action_name,
    const std::string & target)
  {
    factory.registerSimpleAction(
      bt_node_name,
      [this, bt_node_name, action_name, target](BT::TreeNode &) {
        RCLCPP_INFO(
          get_logger(),
          "BT node '%s' requesting robot action '%s'",
          bt_node_name.c_str(),
          action_name.c_str());

        const bool success = call_robot_action(action_name, target, "{}");
        return success ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
      });
  }

  bool call_robot_action(
    const std::string & action_name,
    const std::string & target,
    const std::string & parameters_json)
  {
    if (!robot_action_client_->wait_for_service(kServiceWaitTimeout)) {
      RCLCPP_ERROR(
        get_logger(),
        "Robot action service /robot_action is unavailable after waiting %ld seconds",
        static_cast<long>(kServiceWaitTimeout.count()));
      return false;
    }

    auto request = std::make_shared<ExecuteRobotAction::Request>();
    request->action_name = action_name;
    request->target = target;
    request->parameters_json = parameters_json;

    RCLCPP_INFO(
      get_logger(),
      "Sending /robot_action request: action_name='%s', target='%s', parameters_json='%s'",
      request->action_name.c_str(),
      request->target.c_str(),
      request->parameters_json.c_str());

    auto response_future = robot_action_client_->async_send_request(request);
    const auto future_status = response_future.wait_for(kServiceResponseTimeout);

    if (future_status != std::future_status::ready) {
      RCLCPP_ERROR(
        get_logger(),
        "Timed out waiting for /robot_action response for action '%s'",
        action_name.c_str());
      return false;
    }

    const auto response = response_future.get();
    if (response->success) {
      RCLCPP_INFO(
        get_logger(),
        "/robot_action succeeded for action '%s': %s",
        action_name.c_str(),
        response->message.c_str());
      return true;
    }

    RCLCPP_ERROR(
      get_logger(),
      "/robot_action failed for action '%s': %s",
      action_name.c_str(),
      response->message.c_str());
    return false;
  }

  void publish_status(
    const std::string & task_id,
    const std::string & current_step,
    const std::string & status,
    const std::string & message)
  {
    pai_task_msgs::msg::TaskStatus task_status;
    task_status.task_id = task_id;
    task_status.current_step = current_step;
    task_status.status = status;
    task_status.message = message;
    task_status_publisher_->publish(task_status);
  }

  rclcpp::CallbackGroup::SharedPtr robot_action_callback_group_;
  rclcpp::Client<ExecuteRobotAction>::SharedPtr robot_action_client_;
  rclcpp::Subscription<pai_task_msgs::msg::TaskPlan>::SharedPtr task_plan_subscription_;
  rclcpp::Publisher<pai_task_msgs::msg::TaskStatus>::SharedPtr task_status_publisher_;
};

}  // namespace pai_bt_executor_cpp

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::executors::MultiThreadedExecutor executor;
  auto node = std::make_shared<pai_bt_executor_cpp::BtExecutorNode>();
  executor.add_node(node);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}
