file(GLOB_RECURSE SRCS Source/*.cpp Source/*.hpp)
file(GLOB_RECURSE RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Resources/**)

add_executable(Game ${SRCS} ${RESOURCES})
target_include_directories(Game
        PUBLIC ${SRC_DIR}
)

target_link_libraries(Game
        PUBLIC XYZ.Engine
        PRIVATE XYZ.Resource.Locator.Bundle
        PRIVATE XYZ.Graphics.Renderer.OpenGL
        PRIVATE XYZ.Graphics.Mesh.Obj
        PRIVATE XYZ.Graphics.Texture.Stbi
        PRIVATE XYZ.Graphics.Window.GLFW
        PRIVATE XYZ.Input.Keyboard.GLFW
        PRIVATE XYZ.Input.Mouse.GLFW
        PRIVATE XYZ.Scene.Manager.Octree
        PRIVATE "-framework CoreFoundation"
)

set_target_properties(Game PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
        
        MACOSX_BUNDLE_BUNDLE_NAME "The Game"
        MACOSX_BUNDLE_COPYRIGHT "Rogiel Sulzbach (c) 2017"
        
        MACOSX_BUNDLE_LONG_VERSION_STRING 1.0.0
        MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
)
foreach(file ${RESOURCES})
    file(RELATIVE_PATH rel_file ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${file})
    get_filename_component(resources_dir ${rel_file} DIRECTORY)
    set_source_files_properties(${file} PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources/${resources_dir}
            HEADER_FILE_ONLY ON
    )
endforeach()

add_custom_target(Game.Resources)

find_package(Blender)

if(Blender_FOUND)
    file(GLOB_RECURSE BLENDER_RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/Resources/Objects/*.blend")
    foreach(blender_file ${BLENDER_RESOURCES})
        get_filename_component(blender_file_name ${blender_file} NAME_WE)
        
        add_custom_target(
                Game.Resource.Object.${blender_file_name}
                COMMAND ${Blender_BLENDER_EXECUTABLE} "${blender_file}" --background --python "${CMAKE_SOURCE_DIR}/bin/bbake.py" -- "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
                SOURCES ${blender_file}
        )
        add_dependencies(Game.Resources Game.Resource.Object.${blender_file_name})
    endforeach()
endif()

if (APPLE)
    set(APPS ${CMAKE_CURRENT_BINARY_DIR}/Game.app)
    set(DIRS )
    
    install(CODE "
   include(BundleUtilities)
   fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
   " COMPONENT Runtime)
    
    set_target_properties(Game PROPERTIES
            MACOSX_BUNDLE YES
    )
endif ()
