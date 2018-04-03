list_current_sources_and_subdirs(SRCS DIRS)

add_library(XYZ.Terrain.Manager.Octree STATIC ${SRCS})
target_include_directories(XYZ.Terrain.Manager.Octree
        PRIVATE $<TARGET_PROPERTY:XYZ.Engine,INTERFACE_INCLUDE_DIRECTORIES>
)
