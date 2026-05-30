#include "pai_moveit_interface_cpp/motion_planner.hpp"

namespace pai_moveit_interface_cpp
{

MotionPlanner::MotionPlanner(bool use_moveit)
: use_moveit_(use_moveit)
{
}

bool MotionPlanner::moveToHome(std::string & message)
{
  if (use_moveit_) {
    return moveitPlaceholder(message);
  }
  return mockSuccess("move_to_home", "home", message);
}

bool MotionPlanner::moveToPreGrasp(const std::string & target, std::string & message)
{
  if (use_moveit_) {
    return moveitPlaceholder(message);
  }
  return mockSuccess("move_to_pre_grasp", target, message);
}

bool MotionPlanner::moveToGrasp(const std::string & target, std::string & message)
{
  if (use_moveit_) {
    return moveitPlaceholder(message);
  }
  return mockSuccess("move_to_grasp", target, message);
}

bool MotionPlanner::moveToTarget(const std::string & target, std::string & message)
{
  if (use_moveit_) {
    return moveitPlaceholder(message);
  }
  return mockSuccess("move_to_target", target, message);
}

bool MotionPlanner::returnHome(std::string & message)
{
  if (use_moveit_) {
    return moveitPlaceholder(message);
  }
  return mockSuccess("return_home", "home", message);
}

bool MotionPlanner::mockSuccess(
  const std::string & action,
  const std::string & target,
  std::string & message)
{
  message = "MOCK MotionPlanner completed '" + action + "' for target '" + target + "'";
  return true;
}

bool MotionPlanner::moveitPlaceholder(std::string & message) const
{
  message = "MoveIt 2 execution path is not implemented yet.";
  return false;
}

}  // namespace pai_moveit_interface_cpp
