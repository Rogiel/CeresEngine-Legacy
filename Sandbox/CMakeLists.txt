file(GLOB_RECURSE SRCS *.swift)
add_executable(GameEngine.Sandbox ${SRCS})
configure_swift_sources(GameEngine.Sandbox)
