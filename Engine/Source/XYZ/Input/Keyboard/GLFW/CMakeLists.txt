find_package(GLFW REQUIRED)

if(GLFW_FOUND)
    list_current_sources_and_subdirs(SRCS DIRS)

    add_library(XYZ.Input.Keyboard.GLFW STATIC ${SRCS})
    target_include_directories(XYZ.Input.Keyboard.GLFW
            PRIVATE $<TARGET_PROPERTY:XYZ.Engine,INTERFACE_INCLUDE_DIRECTORIES>
            PUBLIC ${GLFW_INCLUDE_DIRS}
    )
    target_link_libraries(XYZ.Input.Keyboard.GLFW
            PRIVATE ${GLFW_LIBRARIES}
    )
endif()
