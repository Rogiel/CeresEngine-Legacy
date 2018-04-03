list_current_sources_and_subdirs(SRCS DIRS)

add_library(XYZ.Graphics.Mesh.Obj STATIC ${SRCS})
target_link_libraries(XYZ.Graphics.Mesh.Obj PRIVATE XYZ.Engine)
target_include_directories(XYZ.Graphics.Mesh.Obj
        PUBLIC ${CMAKE_SOURCE_DIR}/Vendor/tinyobjloader
)
