# Install script for directory: O:/Users/skibo_000/Desktop/bullet3-master/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/BULLET_PHYSICS")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/skibo_000/Documents/GitHub/That80sGame/That80sGame/Bullet-Build/src/Bullet3OpenCL/cmake_install.cmake")
  include("C:/Users/skibo_000/Documents/GitHub/That80sGame/That80sGame/Bullet-Build/src/Bullet3Serialize/Bullet2FileLoader/cmake_install.cmake")
  include("C:/Users/skibo_000/Documents/GitHub/That80sGame/That80sGame/Bullet-Build/src/Bullet3Dynamics/cmake_install.cmake")
  include("C:/Users/skibo_000/Documents/GitHub/That80sGame/That80sGame/Bullet-Build/src/Bullet3Collision/cmake_install.cmake")
  include("C:/Users/skibo_000/Documents/GitHub/That80sGame/That80sGame/Bullet-Build/src/Bullet3Geometry/cmake_install.cmake")
  include("C:/Users/skibo_000/Documents/GitHub/That80sGame/That80sGame/Bullet-Build/src/BulletInverseDynamics/cmake_install.cmake")
  include("C:/Users/skibo_000/Documents/GitHub/That80sGame/That80sGame/Bullet-Build/src/BulletSoftBody/cmake_install.cmake")
  include("C:/Users/skibo_000/Documents/GitHub/That80sGame/That80sGame/Bullet-Build/src/BulletCollision/cmake_install.cmake")
  include("C:/Users/skibo_000/Documents/GitHub/That80sGame/That80sGame/Bullet-Build/src/BulletDynamics/cmake_install.cmake")
  include("C:/Users/skibo_000/Documents/GitHub/That80sGame/That80sGame/Bullet-Build/src/LinearMath/cmake_install.cmake")
  include("C:/Users/skibo_000/Documents/GitHub/That80sGame/That80sGame/Bullet-Build/src/Bullet3Common/cmake_install.cmake")

endif()

