cmake_minimum_required(VERSION 2.6.0)

# pull in the pods macros. See cmake/pods.cmake for documentation
set(POD_NAME libbot-demo)
include(cmake/pods.cmake)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

# create an executable, and make it public
add_executable(rv-libbot-lcmgl-simple src/rv-libbot-lcmgl-simple.cpp)
pods_use_pkg_config_packages(rv-libbot-lcmgl-simple lcm bot2-core)
target_link_libraries(rv-libbot-lcmgl-simple bot2-core bot2-lcmgl-client)
pods_install_executables(rv-libbot-lcmgl-simple)

add_executable(rv-libbot-pose-demo src/rv-libbot-pose-demo.cpp)
pods_use_pkg_config_packages(rv-libbot-pose-demo lcm bot2-core eigen3)
target_link_libraries(rv-libbot-pose-demo bot2-core bot2-lcmgl-client)
pods_install_executables(rv-libbot-pose-demo)

add_executable(rv-libbot-opencv-demo src/rv-libbot-opencv-demo.cpp)
pods_use_pkg_config_packages(rv-libbot-opencv-demo lcm bot2-core eigen3)
target_link_libraries(rv-libbot-opencv-demo 
	bot2-core bot2-lcmgl-client 
	${OpenCV_LIBS}
	rv-cv-bridge-lcm jpeg-utils z)
pods_install_executables(rv-libbot-opencv-demo)
