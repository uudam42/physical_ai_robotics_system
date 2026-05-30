#include "pai_moveit_interface_cpp/motion_planner.hpp"

#include "rclcpp/logging.hpp"

namespace pai_moveit_interface_cpp
{

MotionPlanner::MotionPlanner(
  bool use_moveit,
  const std::string & planning_group,
  rclcpp::Logger logger)
: use_moveit_(use_moveit),
  planning_group_(planning_group),
  logger_(logger)
{
}

bool MotionPlanner::moveToHome(std::string & message)
{
  if (use_moveit_) {
    return planToNamedTarget("home", message);
  }
  return mockSuccess("move_to_home", "home", message);
}

bool MotionPlanner::moveToPreGrasp(const std::string & target, std::string & message)
{
  if (use_moveit_) {
    return planToPlaceholderPose("move_to_pre_grasp", target, message);
  }
  return mockSuccess("move_to_pre_grasp", target, message);
}

bool MotionPlanner::moveToGrasp(const std::string & target, std::string & message)
{
  if (use_moveit_) {
    return planToPlaceholderPose("move_to_grasp", target, message);
  }
  return mockSuccess("move_to_grasp", target, message);
}

bool MotionPlanner::moveToTarget(const std::string & target, std::string & message)
{
  if (use_moveit_) {
    return planToPlaceholderPose("move_to_target", target, message);
  }
  return mockSuccess("move_to_target", target, message);
}

bool MotionPlanner::returnHome(std::string & message)
{
  if (use_moveit_) {
    return planToNamedTarget("home", message);
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

bool MotionPlanner::moveitUnavailable(std::string & message) const
{
  message = "MoveIt 2 support is disabled at build time. Rebuild with BUILD_WITH_MOVEIT=ON.";
  return false;
}

bool MotionPlanner::planToNamedTarget(const std::string & named_target, std::string & message)
{
#ifdef PAI_BUILD_WITH_MOVEIT
  (void)named_target;
  message = "MoveIt 2 path is compiled but robot model / planning group is not configured yet.";
  RCLCPP_WARN(
    logger_,
    "MoveIt 2 named target planning requested for '%s' with planning_group='%s', but "
    "runtime robot model configuration is not implemented yet",
    named_target.c_str(),
    planning_group_.c_str());
  return false;
#else
  (void)named_target;
  return moveitUnavailable(message);
#endif
}

bool MotionPlanner::planToPlaceholderPose(
  const std::string & action,
  const std::string & target,
  std::string & message)
{
#ifdef PAI_BUILD_WITH_MOVEIT
  message = "MoveIt 2 path is compiled but robot model / planning group is not configured yet.";
  RCLCPP_WARN(
    logger_,
    "MoveIt 2 pose planning requested for action='%s', target='%s', planning_group='%s', but "
    "runtime robot model configuration is not implemented yet",
    action.c_str(),
    target.c_str(),
    planning_group_.c_str());
  return false;
#else
  (void)action;
  (void)target;
  return moveitUnavailable(message);
#endif
}

}  // namespace pai_moveit_interface_cpp
