#include "pai_moveit_interface_cpp/gripper_controller.hpp"

namespace pai_moveit_interface_cpp
{

GripperController::GripperController(bool use_moveit)
: use_moveit_(use_moveit)
{
}

bool GripperController::open(std::string & message)
{
  if (use_moveit_) {
    return moveitPlaceholder(message);
  }
  return mockSuccess("open_gripper", message);
}

bool GripperController::close(std::string & message)
{
  if (use_moveit_) {
    return moveitPlaceholder(message);
  }
  return mockSuccess("close_gripper", message);
}

bool GripperController::mockSuccess(const std::string & action, std::string & message) const
{
  message = "MOCK GripperController completed '" + action + "'";
  return true;
}

bool GripperController::moveitPlaceholder(std::string & message) const
{
#ifdef PAI_BUILD_WITH_MOVEIT
  message = "MoveIt 2 path is compiled but robot model / planning group is not configured yet.";
#else
  message = "MoveIt 2 support is disabled at build time. Rebuild with BUILD_WITH_MOVEIT=ON.";
#endif
  return false;
}

}  // namespace pai_moveit_interface_cpp
