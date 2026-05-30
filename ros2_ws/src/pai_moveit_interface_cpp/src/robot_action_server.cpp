#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "pai_task_msgs/srv/execute_robot_action.hpp"
#include "rclcpp/rclcpp.hpp"

namespace pai_moveit_interface_cpp
{

class RobotActionServer : public rclcpp::Node
{
public:
  RobotActionServer()
  : Node("robot_action_server"),
    supported_actions_{
      "check_object",
      "move_to_home",
      "move_to_pre_grasp",
      "move_to_grasp",
      "move_to_target",
      "open_gripper",
      "close_gripper",
      "return_home"}
  {
    service_ = create_service<pai_task_msgs::srv::ExecuteRobotAction>(
      "/robot_action",
      [this](
        const std::shared_ptr<pai_task_msgs::srv::ExecuteRobotAction::Request> request,
        std::shared_ptr<pai_task_msgs::srv::ExecuteRobotAction::Response> response) {
        handle_request(request, response);
      });

    RCLCPP_INFO(get_logger(), "Mock robot action server started on /robot_action");
  }

private:
  void handle_request(
    const std::shared_ptr<pai_task_msgs::srv::ExecuteRobotAction::Request> request,
    std::shared_ptr<pai_task_msgs::srv::ExecuteRobotAction::Response> response)
  {
    RCLCPP_INFO(
      get_logger(),
      "Received robot action request: action_name='%s', target='%s', parameters_json='%s'",
      request->action_name.c_str(),
      request->target.c_str(),
      request->parameters_json.c_str());

    if (is_supported_action(request->action_name)) {
      response->success = true;
      response->message =
        "Mock action '" + request->action_name + "' accepted for target '" +
        request->target + "'";
      RCLCPP_INFO(get_logger(), "%s", response->message.c_str());
      return;
    }

    response->success = false;
    response->message = "Unsupported robot action: " + request->action_name;
    RCLCPP_WARN(get_logger(), "%s", response->message.c_str());
  }

  bool is_supported_action(const std::string & action_name) const
  {
    return std::find(
      supported_actions_.begin(),
      supported_actions_.end(),
      action_name) != supported_actions_.end();
  }

  std::vector<std::string> supported_actions_;
  rclcpp::Service<pai_task_msgs::srv::ExecuteRobotAction>::SharedPtr service_;
};

}  // namespace pai_moveit_interface_cpp

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<pai_moveit_interface_cpp::RobotActionServer>());
  rclcpp::shutdown();
  return 0;
}
