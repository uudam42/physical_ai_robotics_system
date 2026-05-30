#include "pai_moveit_interface_cpp/robot_action_server.hpp"

namespace pai_moveit_interface_cpp
{

RobotActionServer::RobotActionServer()
: Node("robot_action_server"),
  use_moveit_(false),
  planning_group_("arm")
{
  use_moveit_ = declare_parameter<bool>("use_moveit", false);
  planning_group_ = declare_parameter<std::string>("planning_group", "arm");
  motion_planner_ = std::make_unique<MotionPlanner>(
    use_moveit_,
    planning_group_,
    get_logger());
  gripper_controller_ = std::make_unique<GripperController>(use_moveit_);

  service_ = create_service<ExecuteRobotAction>(
    "/robot_action",
    [this](
      const std::shared_ptr<ExecuteRobotAction::Request> request,
      std::shared_ptr<ExecuteRobotAction::Response> response) {
      handleRequest(request, response);
    });

  const char * mode = use_moveit_ ? "MOVEIT_PLACEHOLDER" : "MOCK";
  RCLCPP_INFO(get_logger(), "Robot action server started on /robot_action");
  RCLCPP_INFO(get_logger(), "Robot action server mode: %s", mode);
  RCLCPP_INFO(get_logger(), "Planning group: %s", planning_group_.c_str());
}

void RobotActionServer::handleRequest(
  const std::shared_ptr<ExecuteRobotAction::Request> request,
  std::shared_ptr<ExecuteRobotAction::Response> response)
{
  RCLCPP_INFO(
    get_logger(),
    "Received robot action request: action_name='%s', target='%s', parameters_json='%s'",
    request->action_name.c_str(),
    request->target.c_str(),
    request->parameters_json.c_str());

  std::string message;
  response->success = executeAction(*request, message);
  response->message = message;

  if (response->success) {
    RCLCPP_INFO(get_logger(), "%s", response->message.c_str());
  } else {
    RCLCPP_WARN(get_logger(), "%s", response->message.c_str());
  }
}

bool RobotActionServer::executeAction(
  const ExecuteRobotAction::Request & request,
  std::string & message)
{
  if (request.action_name == "check_object") {
    return checkObject(request.target, message);
  }
  if (request.action_name == "move_to_home") {
    return motion_planner_->moveToHome(message);
  }
  if (request.action_name == "move_to_pre_grasp") {
    return motion_planner_->moveToPreGrasp(request.target, message);
  }
  if (request.action_name == "move_to_grasp") {
    return motion_planner_->moveToGrasp(request.target, message);
  }
  if (request.action_name == "move_to_target") {
    return motion_planner_->moveToTarget(request.target, message);
  }
  if (request.action_name == "return_home") {
    return motion_planner_->returnHome(message);
  }
  if (request.action_name == "open_gripper") {
    return gripper_controller_->open(message);
  }
  if (request.action_name == "close_gripper") {
    return gripper_controller_->close(message);
  }

  message = "Unsupported robot action: " + request.action_name;
  return false;
}

bool RobotActionServer::checkObject(const std::string & target, std::string & message) const
{
  message = "MOCK perception check found object '" + target + "'";
  return true;
}

}  // namespace pai_moveit_interface_cpp

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<pai_moveit_interface_cpp::RobotActionServer>());
  rclcpp::shutdown();
  return 0;
}
