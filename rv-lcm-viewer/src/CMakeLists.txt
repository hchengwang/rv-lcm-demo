#add_definitions(-std=gnu99)

link_libraries(boost_thread)

add_executable(rv-lcm-viewer rv_lcm_viewer.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(IMAGE_UTIL_RENDERER image-util-renderer )

if(IMAGE_UTIL_RENDERER_FOUND) 
    message("\n")
    message("image-util-renderer found.")
    message("\n")
    pods_use_pkg_config_packages(rv-lcm-viewer 
      bot2-vis 
      bot2-param-client
      bot2-lcmgl-renderer 
      bot2-frames-renderers
      image-util-renderer  
      )
endif()

pods_install_executables(rv-lcm-viewer)
