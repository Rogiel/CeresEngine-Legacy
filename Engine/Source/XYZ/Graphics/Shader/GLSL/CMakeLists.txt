list_current_sources_and_subdirs(SRCS DIRS)

add_library(XYZ.Graphics.Shader.GLSL STATIC ${SRCS})
target_include_directories(XYZ.Graphics.Shader.GLSL
        PRIVATE $<TARGET_PROPERTY:XYZ.Engine,INTERFACE_INCLUDE_DIRECTORIES>
)

foreach(subdir ${DIRS})
    add_subdirectory(${subdir})
endforeach()
