#ifndef PAI_MOVEIT_INTERFACE_CPP__MOTION_PLANNER_HPP_
#define PAI_MOVEIT_INTERFACE_CPP__MOTION_PLANNER_HPP_

#include <string>

namespace pai_moveit_interface_cpp
{

class MotionPlanner
{
public:
  explicit MotionPlanner(bool use_moveit = false);

  bool moveToHome(std::string & message);
  bool moveToPreGrasp(const std::string & target, std::string & message);
  bool moveToGrasp(const std::string & target, std::string & message);
  bool moveToTarget(const std::string & target, std::string & message);
  bool returnHome(std::string & message);

private:
  bool mockSuccess(const std::string & action, const std::string & target, std::string & message);
  bool moveitPlaceholder(std::string & message) const;

  bool use_moveit_;
};

}  // namespace pai_moveit_interface_cpp

#endif  // PAI_MOVEIT_INTERFACE_CPP__MOTION_PLANNER_HPP_
