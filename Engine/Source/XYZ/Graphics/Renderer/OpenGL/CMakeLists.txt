find_package(OpenGL REQUIRED)
find_package(Glew REQUIRED)

if(OPENGL_FOUND AND GLEW_FOUND)
    list_current_sources_and_subdirs(SRCS DIRS)

    add_library(XYZ.Graphics.Renderer.OpenGL STATIC ${SRCS})
    target_include_directories(XYZ.Graphics.Renderer.OpenGL
            PRIVATE $<TARGET_PROPERTY:XYZ.Engine,INTERFACE_INCLUDE_DIRECTORIES>
            PUBLIC ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS}
    )
    target_link_libraries(XYZ.Graphics.Renderer.OpenGL
            PRIVATE ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}
    )
        
    foreach(subdir ${DIRS})
        add_subdirectory(${subdir})
    endforeach()
endif()
