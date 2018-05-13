# Install script for directory: /home/cmb/esp/esp-idf/examples/open62541/components/open62541/open62541/arch

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/cmb/esp/esp-idf/examples/open62541/build/open62541/arch/posix/cmake_install.cmake")
  include("/home/cmb/esp/esp-idf/examples/open62541/build/open62541/arch/win32/cmake_install.cmake")
  include("/home/cmb/esp/esp-idf/examples/open62541/build/open62541/arch/arduino/cmake_install.cmake")
  include("/home/cmb/esp/esp-idf/examples/open62541/build/open62541/arch/freertosLWIP/cmake_install.cmake")
  include("/home/cmb/esp/esp-idf/examples/open62541/build/open62541/arch/vxworks/cmake_install.cmake")
  include("/home/cmb/esp/esp-idf/examples/open62541/build/open62541/arch/eCos/cmake_install.cmake")

endif()

