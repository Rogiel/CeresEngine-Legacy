list_current_sources_and_subdirs(SRCS DIRS)

add_library(XYZ.Terrain.Noise STATIC ${SRCS})
target_link_libraries(XYZ.Terrain.Noise
        PRIVATE XYZ.Engine
)
