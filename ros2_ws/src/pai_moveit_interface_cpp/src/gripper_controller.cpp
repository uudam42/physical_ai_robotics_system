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
  message = "MoveIt 2 execution path is not implemented yet.";
  return false;
}

}  // namespace pai_moveit_interface_cpp
