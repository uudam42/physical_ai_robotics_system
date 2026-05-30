#ifndef PAI_MOVEIT_INTERFACE_CPP__GRIPPER_CONTROLLER_HPP_
#define PAI_MOVEIT_INTERFACE_CPP__GRIPPER_CONTROLLER_HPP_

#include <string>

namespace pai_moveit_interface_cpp
{

class GripperController
{
public:
  explicit GripperController(bool use_moveit = false);

  bool open(std::string & message);
  bool close(std::string & message);

private:
  bool mockSuccess(const std::string & action, std::string & message) const;
  bool moveitPlaceholder(std::string & message) const;

  bool use_moveit_;
};

}  // namespace pai_moveit_interface_cpp

#endif  // PAI_MOVEIT_INTERFACE_CPP__GRIPPER_CONTROLLER_HPP_
