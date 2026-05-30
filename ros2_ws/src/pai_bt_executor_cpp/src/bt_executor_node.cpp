#include <exception>
#include <memory>
#include <string>

#include "behaviortree_cpp/bt_factory.h"
#include "pai_task_msgs/msg/task_plan.hpp"
#include "pai_task_msgs/msg/task_status.hpp"
#include "rclcpp/rclcpp.hpp"

namespace pai_bt_executor_cpp
{

class MockBtAction : public BT::SyncActionNode
{
public:
  MockBtAction(
    const std::string & name,
    const BT::NodeConfig & config,
    rclcpp::Logger logger)
  : BT::SyncActionNode(name, config),
    logger_(logger)
  {
  }

  static BT::PortsList providedPorts()
  {
    return {};
  }

  BT::NodeStatus tick() override
  {
    RCLCPP_INFO(logger_, "Executing BT action: %s", name().c_str());
    return BT::NodeStatus::SUCCESS;
  }

private:
  rclcpp::Logger logger_;
};

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

    task_plan_subscription_ =
      create_subscription<pai_task_msgs::msg::TaskPlan>(
      "/task_plan",
      10,
      [this](pai_task_msgs::msg::TaskPlan::SharedPtr msg) {
        handle_task_plan(*msg);
      });

    RCLCPP_INFO(get_logger(), "C++ BehaviorTree executor node started");
    RCLCPP_INFO(get_logger(), "Listening on /task_plan and publishing to /task_status");
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
      auto factory = create_factory();
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

  BT::BehaviorTreeFactory create_factory()
  {
    BT::BehaviorTreeFactory factory;
    register_mock_action(factory, "CheckObject");
    register_mock_action(factory, "MoveToPreGrasp");
    register_mock_action(factory, "OpenGripper");
    register_mock_action(factory, "MoveToGrasp");
    register_mock_action(factory, "CloseGripper");
    register_mock_action(factory, "MoveToTarget");
    register_mock_action(factory, "ReturnHome");
    return factory;
  }

  void register_mock_action(
    BT::BehaviorTreeFactory & factory,
    const std::string & node_name)
  {
    factory.registerNodeType<MockBtAction>(node_name, get_logger());
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

  rclcpp::Subscription<pai_task_msgs::msg::TaskPlan>::SharedPtr task_plan_subscription_;
  rclcpp::Publisher<pai_task_msgs::msg::TaskStatus>::SharedPtr task_status_publisher_;
};

}  // namespace pai_bt_executor_cpp

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<pai_bt_executor_cpp::BtExecutorNode>());
  rclcpp::shutdown();
  return 0;
}
