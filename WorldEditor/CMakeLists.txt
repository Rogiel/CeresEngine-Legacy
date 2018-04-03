# Next lines needed for building all Qt projects
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets QUIET)

if(Qt5Widgets_FOUND)
set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources)
set(GAME_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Game/Resources)

file(GLOB_RECURSE SRCS Source/*.cpp Source/*.hpp)
file(GLOB_RECURSE UIS Source/*.ui)
file(GLOB_RECURSE QRCS_SRC *.qrc)
file(GLOB_RECURSE RESOURCES ${RESOURCES_DIR}/**)
file(GLOB_RECURSE GAME_RESOURCES ${GAME_RESOURCES_DIR}/**)

# Generate code from ui files
qt5_wrap_ui(UI_HEADERS ${UIS})

# Generate rules for building source files from the resources
qt5_add_resources(QRCS ${QRCS_SRC})

add_executable(XYZ.WorldEditor ${SRCS} ${UI_HEADERS} ${QRCS} ${RESOURCES} ${GAME_RESOURCES})
target_include_directories(XYZ.WorldEditor
        PUBLIC ${SRC_DIR}
        PUBLIC ${GLM_INCLUDE_DIRS}
        PRIVATE Source/
)

target_link_libraries(XYZ.WorldEditor
        PRIVATE Qt5::Widgets
        
        PUBLIC XYZ.Engine
        
        PRIVATE XYZ.Resource.Locator.Bundle
        
        PRIVATE XYZ.Graphics.Renderer.OpenGL
        PRIVATE XYZ.Graphics.Mesh.Obj
        PRIVATE XYZ.Graphics.Texture.Stbi
        PRIVATE XYZ.Graphics.Window.GLFW
        
        PRIVATE XYZ.Audio.OpenAL
        PRIVATE XYZ.Audio.Loader.OggVorbis
        
        PRIVATE XYZ.Input.Keyboard.GLFW
        PRIVATE XYZ.Input.Mouse.GLFW
        
        PRIVATE XYZ.Scene.Manager.Octree
        
        PRIVATE "-framework CoreFoundation"
        PRIVATE "-framework AudioToolbox"
)

set_target_properties(XYZ.WorldEditor PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
        
        MACOSX_BUNDLE_BUNDLE_NAME "XYZ World Editor"
        MACOSX_BUNDLE_COPYRIGHT "Rogiel Sulzbach (c) 2017"
        
        MACOSX_BUNDLE_LONG_VERSION_STRING 1.0.0
        MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
        
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.rogiel.xyz.WorldEditor"
        MACOSX_BUNDLE_ICON_FILE      AppIcon.icns
        
        OUTPUT_NAME "World Editor"
)

foreach(file ${RESOURCES})
    file(RELATIVE_PATH rel_file ${RESOURCES_DIR} ${file})
    get_filename_component(resources_dir ${rel_file} DIRECTORY)
    set_source_files_properties(${file} PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources/${resources_dir}
            HEADER_FILE_ONLY ON
    )
endforeach()

foreach(file ${GAME_RESOURCES})
    file(RELATIVE_PATH rel_file ${GAME_RESOURCES_DIR} ${file})
    get_filename_component(resources_dir ${rel_file} DIRECTORY)
    set_source_files_properties(${file} PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources/${resources_dir}
            HEADER_FILE_ONLY ON
    )
endforeach()

if (APPLE)
#    set(APPS "${CMAKE_CURRENT_BINARY_DIR}/World Editor.app")
#    set(DIRS )
#
#    install(CODE "
#   include(BundleUtilities)
#   fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
#   " COMPONENT Runtime)

    set_target_properties(XYZ.WorldEditor PROPERTIES
            MACOSX_BUNDLE YES
    )
endif ()
endif()