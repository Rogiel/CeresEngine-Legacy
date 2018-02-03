file(GLOB_RECURSE SRCS SGLMath/Sources/SGLMath/*.swift)
add_library(SGLMath STATIC ${SRCS})
configure_swift_sources(SGLMath)

file(GLOB_RECURSE SRCS SGLOpenGL/Sources/SGLOpenGL/*.swift)
add_library(SGLOpenGL STATIC ${SRCS})
configure_swift_sources(SGLOpenGL)

file(GLOB_RECURSE SRCS SGLImage/Sources/SGLImage/*.swift)
add_library(SGLImage STATIC ${SRCS})
configure_swift_sources(SGLImage)

add_library(CGLFW3 INTERFACE)
#target_include_directories(CGLFW3 INTERFACE /usr/local/include)