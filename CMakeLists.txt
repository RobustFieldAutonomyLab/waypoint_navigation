cmake_minimum_required(VERSION 2.8.3)
project(waypoint_navigation)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3")

find_package(catkin REQUIRED COMPONENTS
  tf
  roscpp
  rospy
)

find_package(PythonLibs REQUIRED)

catkin_package()

# include directories
include_directories(
	${catkin_INCLUDE_DIRS}
	${PYTHON_INCLUDE_DIRS}
)

add_executable(goal_publisher src/goal_publisher.cpp)
target_link_libraries(goal_publisher ${catkin_LIBRARIES})

target_link_libraries(goal_publisher ${PYTHON_LIBRARIES})
