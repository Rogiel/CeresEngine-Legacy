cmake_minimum_required(VERSION 3.8)
project(GameEngine)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.10)

include(SwiftTarget)

add_subdirectory(Vendor)
add_subdirectory(Engine)
add_subdirectory(WorldEditor)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Game/CMakeLists.txt)
    add_subdirectory(Game)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Sandbox/CMakeLists.txt)
    add_subdirectory(Sandbox)
endif()