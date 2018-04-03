list_current_sources_and_subdirs(SRCS DIRS)

add_library(XYZ.Graphics.Model.Loader.COLLADA STATIC ${SRCS})
target_link_libraries(XYZ.Graphics.Model.Loader.COLLADA PRIVATE XYZ.Engine)

