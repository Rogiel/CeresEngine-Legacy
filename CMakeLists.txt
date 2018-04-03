cmake_minimum_required(VERSION 3.8)
project(Engine)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)

include(SourceAndDirectoryListing)

#set(NANOGUI_BUILD_EXAMPLE FALSE)
#set(NANOGUI_BUILD_SHARED FALSE)
#set(NANOGUI_BUILD_PYTHON FALSE)
#set(NANOGUI_USE_GLAD FALSE)
#set(NANOGUI_INSTALL FALSE)
#add_subdirectory(Vendor/nanogui)

#add_subdirectory(Vendor/libnoise)

add_subdirectory(Engine)

option(XYZ_ENABLE_WORLD_EDITOR "Enable compilation of the XYZ World Editor" ON)
if(XYZ_ENABLE_WORLD_EDITOR)
    add_subdirectory(WorldEditor)
endif()
add_subdirectory(Game)
