cmake_minimum_required(VERSION 3.8)
project(pai_moveit_interface_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(pai_task_msgs REQUIRED)
find_package(rclcpp REQUIRED)

option(BUILD_WITH_MOVEIT "Build pai_moveit_interface_cpp with MoveIt 2 support" OFF)

if(BUILD_WITH_MOVEIT)
  find_package(moveit_ros_planning_interface REQUIRED)
endif()

add_executable(robot_action_server
  src/gripper_controller.cpp
  src/motion_planner.cpp
  src/robot_action_server.cpp
)
target_compile_features(robot_action_server PUBLIC cxx_std_17)
target_include_directories(robot_action_server PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(robot_action_server
  pai_task_msgs
  rclcpp
)

if(BUILD_WITH_MOVEIT)
  target_compile_definitions(robot_action_server PRIVATE PAI_BUILD_WITH_MOVEIT)
  ament_target_dependencies(robot_action_server
    moveit_ros_planning_interface
  )
endif()

install(TARGETS robot_action_server
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

ament_package()
