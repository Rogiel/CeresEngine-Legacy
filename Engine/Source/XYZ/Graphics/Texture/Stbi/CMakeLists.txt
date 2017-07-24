list_current_sources_and_subdirs(SRCS DIRS)

add_library(XYZ.Graphics.Texture.Stbi STATIC ${SRCS})
target_include_directories(XYZ.Graphics.Texture.Stbi
        PRIVATE $<TARGET_PROPERTY:XYZ.Engine,INTERFACE_INCLUDE_DIRECTORIES>
        PUBLIC ${CMAKE_SOURCE_DIR}/Vendor/stb
)

foreach (subdir ${DIRS})
    add_subdirectory(${subdir})
endforeach ()
