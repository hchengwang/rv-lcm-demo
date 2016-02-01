cmake_minimum_required(VERSION 2.6.0)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

# pull in the pods macros. See cmake/pods.cmake for documentation
set(POD_NAME libbot-demo)
include(cmake/pods.cmake)

# create an executable, and make it public
add_executable(rv-lcmgl-maze src/rv-lcmgl-maze.cpp)
pods_use_pkg_config_packages(rv-lcmgl-maze lcm bot2-core eigen3)
target_link_libraries(rv-lcmgl-maze bot2-core bot2-lcmgl-client)
pods_install_executables(rv-lcmgl-maze)
