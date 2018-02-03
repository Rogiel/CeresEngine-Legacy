file(GLOB_RECURSE SRCS *.swift)
add_executable(Game.Sandbox ${SRCS})
target_link_libraries(Game.Sandbox PUBLIC Game)
configure_swift_sources(Game.Sandbox)
