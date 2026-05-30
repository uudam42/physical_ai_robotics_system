cmake_minimum_required(VERSION 3.8)
project(pai_bt_executor_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(behaviortree_cpp REQUIRED)
find_package(pai_task_msgs REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(bt_executor_node src/bt_executor_node.cpp)
target_compile_features(bt_executor_node PUBLIC cxx_std_17)
target_include_directories(bt_executor_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(bt_executor_node
  behaviortree_cpp
  pai_task_msgs
  rclcpp
)

install(TARGETS bt_executor_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

ament_package()
