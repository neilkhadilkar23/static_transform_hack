cmake_minimum_required(VERSION 3.5)
project(neil_localization_hack)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};/usr/share/cmake/geographiclib")
# uncomment the following section in order to fill in
# further dependencies manually.
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)

#GEOLIB INCLUDES
find_package(GeographicLib REQUIRED)
set(dependencies
    rclcpp 
    std_msgs 
    sensor_msgs 
    geometry_msgs
    nav_msgs
    tf2
    tf2_ros
    
)

include_directories(include)
#GEO TRANSFORM
add_executable(geo_transform src/geo_transform.cpp)
target_link_libraries(geo_transform ${GeographicLib_LIBRARIES})
#END GEOLIB INCLUDES
ament_target_dependencies(geo_transform ${dependencies})


target_include_directories(geo_transform PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

install(TARGETS geo_transform
  DESTINATION lib/${PROJECT_NAME})

#GEO TRANSFORM


#PATH PUBLISHER
add_executable(path_publisher src/path_publisher.cpp)
#END GEOLIB INCLUDES
ament_target_dependencies(path_publisher ${dependencies})


target_include_directories(path_publisher PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

install(TARGETS path_publisher
  DESTINATION lib/${PROJECT_NAME})
# Install launch files.
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)
#PATH PUBLISHER
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
