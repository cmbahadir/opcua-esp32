# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)


include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(EXTRA_COMPONENTS_DIRS "components/open62541lib" "components/ethernet" "components/model")
project(opcua_esp32)