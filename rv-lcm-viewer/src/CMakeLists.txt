#add_definitions(-std=gnu99)

add_executable(rv-lcm-viewer rv_lcm_viewer.cpp)
set(REQUIRED_LIBS bot2-param-client bot2-frames)  
pods_use_pkg_config_packages(rv-lcm-viewer ${REQUIRED_LIBS})
pods_use_pkg_config_packages(rv-lcm-viewer 
      bot2-vis 
      bot2-param-client
      bot2-lcmgl-renderer 
      bot2-frames-renderers
      image-util-renderer  
)
pods_install_executables(rv-lcm-viewer)
