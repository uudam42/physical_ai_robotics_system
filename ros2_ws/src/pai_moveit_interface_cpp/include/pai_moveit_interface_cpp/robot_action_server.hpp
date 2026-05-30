#ifndef PAI_MOVEIT_INTERFACE_CPP__ROBOT_ACTION_SERVER_HPP_
#define PAI_MOVEIT_INTERFACE_CPP__ROBOT_ACTION_SERVER_HPP_

#include <memory>
#include <string>

#include "pai_moveit_interface_cpp/gripper_controller.hpp"
#include "pai_moveit_interface_cpp/motion_planner.hpp"
#include "pai_task_msgs/srv/execute_robot_action.hpp"
#include "rclcpp/rclcpp.hpp"

namespace pai_moveit_interface_cpp
{

class RobotActionServer : public rclcpp::Node
{
public:
  RobotActionServer();

private:
  using ExecuteRobotAction = pai_task_msgs::srv::ExecuteRobotAction;

  void handleRequest(
    const std::shared_ptr<ExecuteRobotAction::Request> request,
    std::shared_ptr<ExecuteRobotAction::Response> response);

  bool executeAction(const ExecuteRobotAction::Request & request, std::string & message);
  bool checkObject(const std::string & target, std::string & message) const;

  bool use_moveit_;
  std::string planning_group_;
  std::unique_ptr<MotionPlanner> motion_planner_;
  std::unique_ptr<GripperController> gripper_controller_;
  rclcpp::Service<ExecuteRobotAction>::SharedPtr service_;
};

}  // namespace pai_moveit_interface_cpp

#endif  // PAI_MOVEIT_INTERFACE_CPP__ROBOT_ACTION_SERVER_HPP_
