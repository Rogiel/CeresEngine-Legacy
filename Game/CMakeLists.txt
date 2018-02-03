set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)

file(GLOB_RECURSE SRCS Sources/*.swift)
add_library(Game STATIC ${SRCS})
target_link_libraries(Game PUBLIC GameEngine)
configure_swift_sources(Game)

file(GLOB_RECURSE SRCS Launcher/*.swift)
file(GLOB_RECURSE RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Resources/**)

add_executable(GameLauncher ${SRCS} ${RESOURCES})
target_link_libraries(GameLauncher
        PUBLIC Game
        PUBLIC GameEngine
        PUBLIC GameEngine.GLFW
        PUBLIC GameEngine.UI)
configure_swift_sources(GameLauncher)

set_target_properties(GameLauncher PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
        
        MACOSX_BUNDLE_BUNDLE_NAME "The Game"
        MACOSX_BUNDLE_COPYRIGHT "Rogiel Sulzbach (c) 2017"
        
        MACOSX_BUNDLE_LONG_VERSION_STRING 1.0.0
        MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
        
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.rogiel.SwiftGameEngine"
        MACOSX_BUNDLE_ICON_FILE      AppIcon.icns
)
foreach(file ${RESOURCES})
    file(RELATIVE_PATH rel_file ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${file})
    get_filename_component(resources_dir ${rel_file} DIRECTORY)
    set_source_files_properties(${file} PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources/${resources_dir}
            HEADER_FILE_ONLY TRUE
    )
    file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/GameLauncher.app/Contents/Resources/${resources_dir})
endforeach()

if (APPLE)
    set(APPS ${CMAKE_CURRENT_BINARY_DIR}/GameLauncher.app)
    set(DIRS )
    
    install(CODE "
   include(BundleUtilities)
   fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
   " COMPONENT Runtime)
    
    set_target_properties(GameLauncher PROPERTIES
            BUNDLE YES
            MACOSX_BUNDLE YES
    )
endif ()

## Resources
add_custom_target(Game.Resources)
find_package(Blender)
if(Blender_FOUND)
    file(GLOB_RECURSE BLENDER_RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Objects/*.blend")
    foreach(blender_file ${BLENDER_RESOURCES})
        get_filename_component(blender_file_name ${blender_file} NAME_WE)
        
        add_custom_target(
                Game.Resource.Object.${blender_file_name}
                COMMAND ${Blender_EXECUTABLE} "${blender_file}" --background --python "${CMAKE_SOURCE_DIR}/Tools/bbake.py" -- "${CMAKE_CURRENT_BINARY_DIR}/GameLauncher.app/Contents/Resources"
                SOURCES ${blender_file}
        )
        add_dependencies(Game.Resources Game.Resource.Object.${blender_file_name})
    endforeach()
endif()

if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Sandbox)
    add_subdirectory(Sandbox)
endif()
