find_package(GLFW REQUIRED)

if(GLFW_FOUND)
    list_current_sources_and_subdirs(SRCS DIRS)

    add_library(XYZ.Graphics.Window.GLFW STATIC ${SRCS})
    target_include_directories(XYZ.Graphics.Window.GLFW
            PRIVATE $<TARGET_PROPERTY:XYZ.Engine,INTERFACE_INCLUDE_DIRECTORIES>
            PUBLIC ${GLFW_INCLUDE_DIRS}
    )
    target_link_libraries(XYZ.Graphics.Window.GLFW
            PRIVATE ${GLFW_LIBRARIES}
    )
        
    foreach(subdir ${DIRS})
        add_subdirectory(${subdir})
    endforeach()
endif()
