cmake_minimum_required(VERSION 3.8)
project(Engine)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -fno-rtti")

include(SourceAndDirectoryListing)

add_subdirectory(Engine)
#add_subdirectory(WorldEditor)
add_subdirectory(Game)
