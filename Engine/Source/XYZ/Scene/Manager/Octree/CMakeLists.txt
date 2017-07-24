list_current_sources_and_subdirs(SRCS DIRS)

add_library(XYZ.Scene.Manager.Octree STATIC ${SRCS})
target_include_directories(XYZ.Scene.Manager.Octree
        PRIVATE $<TARGET_PROPERTY:XYZ.Engine,INTERFACE_INCLUDE_DIRECTORIES>
)

foreach (subdir ${DIRS})
    add_subdirectory(${subdir})
endforeach ()
