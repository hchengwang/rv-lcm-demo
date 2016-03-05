cmake_minimum_required(VERSION 2.6.0)

# pull in the pods macros. See cmake/pods.cmake for documentation
set(POD_NAME libbot-demo)
include(cmake/pods.cmake)

# create an executable, and make it public
add_executable(rv-libbot-lcmgl-simple src/rv-libbot-lcmgl-simple.cpp)
pods_use_pkg_config_packages(rv-libbot-lcmgl-simple lcm bot2-core)
target_link_libraries(rv-libbot-lcmgl-simple bot2-core bot2-lcmgl-client)
pods_install_executables(rv-libbot-lcmgl-simple)
