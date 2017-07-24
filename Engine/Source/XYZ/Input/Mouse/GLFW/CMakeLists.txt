find_package(GLFW REQUIRED)

if(GLFW_FOUND)
    list_current_sources_and_subdirs(SRCS DIRS)

    add_library(XYZ.Input.Mouse.GLFW STATIC ${SRCS})
    target_include_directories(XYZ.Input.Mouse.GLFW
            PRIVATE $<TARGET_PROPERTY:XYZ.Engine,INTERFACE_INCLUDE_DIRECTORIES>
            PUBLIC ${GLFW_INCLUDE_DIRS}
    )
    target_link_libraries(XYZ.Input.Mouse.GLFW
            PRIVATE ${GLFW_LIBRARIES}
    )
endif()
