#ifndef PAI_MOVEIT_INTERFACE_CPP__MOTION_PLANNER_HPP_
#define PAI_MOVEIT_INTERFACE_CPP__MOTION_PLANNER_HPP_

#include <memory>
#include <string>

#include "rclcpp/logger.hpp"

#ifdef PAI_BUILD_WITH_MOVEIT
#include "moveit/move_group_interface/move_group_interface.h"
#endif

namespace pai_moveit_interface_cpp
{

class MotionPlanner
{
public:
  MotionPlanner(
    bool use_moveit = false,
    const std::string & planning_group = "arm",
    rclcpp::Logger logger = rclcpp::get_logger("motion_planner"));

  bool moveToHome(std::string & message);
  bool moveToPreGrasp(const std::string & target, std::string & message);
  bool moveToGrasp(const std::string & target, std::string & message);
  bool moveToTarget(const std::string & target, std::string & message);
  bool returnHome(std::string & message);

private:
  bool mockSuccess(const std::string & action, const std::string & target, std::string & message);
  bool moveitUnavailable(std::string & message) const;
  bool planToNamedTarget(const std::string & named_target, std::string & message);
  bool planToPlaceholderPose(
    const std::string & action,
    const std::string & target,
    std::string & message);

  bool use_moveit_;
  std::string planning_group_;
  rclcpp::Logger logger_;

#ifdef PAI_BUILD_WITH_MOVEIT
  std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface_;
#endif
};

}  // namespace pai_moveit_interface_cpp

#endif  // PAI_MOVEIT_INTERFACE_CPP__MOTION_PLANNER_HPP_
