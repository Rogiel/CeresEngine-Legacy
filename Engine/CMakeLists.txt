file(GLOB_RECURSE SRCS Sources/*.swift Sources/*.c Sources/*.cpp include/*.h include/module.modulemap)
add_library(GameEngine STATIC ${SRCS})
target_link_libraries(GameEngine PUBLIC SGLMath SGLOpenGL SGLImage)
target_include_directories(GameEngine PUBLIC include)

# OpenGL shader recompilation
file(GLOB shader_sources        ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/OpenGL/*.vs)
set(shader_swift_sources)

file(GLOB_RECURSE shader_includes ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/OpenGL/include/*.glsl)

foreach(shader ${shader_sources})
    string(REGEX REPLACE "(.*).vs" "\\1" _shader_name ${shader})
    get_filename_component(shader_name ${_shader_name} NAME)
    
    set(deps ${shader})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/OpenGL/${shader_name}.fs)
        list(APPEND deps ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/OpenGL/${shader_name}.fs)
    endif()
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/OpenGL/${shader_name}.gs)
        list(APPEND deps ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/OpenGL/${shader_name}.gs)
    endif()
    
    add_custom_command(
            OUTPUT              ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Renderer/OpenGL/Shaders/OpenGLShader+${shader_name}.swift
            COMMAND             ${CMAKE_SOURCE_DIR}/Tools/opengl_shader.py ${shader_name}
            MAIN_DEPENDENCY     ${shader}
            DEPENDS             ${deps} ${CMAKE_SOURCE_DIR}/Tools/opengl_shader.py ${shader_includes}
            COMMENT             "Building Swift source for shader ${shader_name}"
    )
    list(APPEND shader_swift_sources ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Renderer/OpenGL/Shaders/OpenGLShader+${shader_name}.swift)

    target_sources(GameEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Renderer/OpenGL/Shaders/OpenGLShader+${shader_name}.swift)
endforeach()

configure_swift_sources(GameEngine)

add_subdirectory(PlugIns)
