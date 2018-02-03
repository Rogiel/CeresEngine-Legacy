file(GLOB_RECURSE SRCS GLFW/*.swift)
add_library(GameEngine.GLFW STATIC ${SRCS})
target_link_libraries(GameEngine.GLFW PUBLIC GameEngine)
target_include_directories(GameEngine.GLFW PUBLIC ../../Vendor/CGLFW3 /usr/local/include)
set_target_properties(GameEngine.GLFW PROPERTIES
        SWIFT_MODULE_NAME GameEngineGLFW)
configure_swift_sources(GameEngine.GLFW)

file(GLOB_RECURSE SRCS UI/*.swift UI/*.c)
add_library(GameEngine.UI STATIC ${SRCS})
target_link_libraries(GameEngine.UI PUBLIC GameEngine)
target_include_directories(GameEngine.UI PUBLIC UI/CNanoVG/include)
set_target_properties(GameEngine.UI PROPERTIES
        SWIFT_MODULE_NAME GameEngineUI)
configure_swift_sources(GameEngine.UI)