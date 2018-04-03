list_current_sources_and_subdirs(SRCS DIRS)

add_library(XYZ.Terrain.Plain STATIC ${SRCS})
target_link_libraries(XYZ.Terrain.Plain
        PRIVATE XYZ.Engine
)
