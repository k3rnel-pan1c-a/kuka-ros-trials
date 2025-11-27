cmake_minimum_required(VERSION 3.8)

project(my_robot_description)

# Use ament instead of catkin
find_package(ament_cmake REQUIRED)

# If you need launch testing (ROS 2 style)
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()

  find_package(launch_testing_ament_cmake REQUIRED)
endif()

# Install resources (configs, meshes, urdf, launch files)
install(
  DIRECTORY config launch meshes urdf rviz
  DESTINATION share/${PROJECT_NAME}
)

# Export the package to be discoverable
ament_package()