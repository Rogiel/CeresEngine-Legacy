find_package(GLM)

if(GLM_FOUND)
    set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
    list_sources_and_subdirs(SRCS DIRS ${SRC_DIR})

    add_library(XYZ.Engine STATIC ${SRCS})
    target_include_directories(XYZ.Engine
            PUBLIC ${SRC_DIR}
            PUBLIC ${GLM_INCLUDE_DIRS}
    )
    
    target_compile_options(XYZ.Engine
            PUBLIC -msse2
    )

    foreach(subdir ${DIRS})
        add_subdirectory(${subdir})
    endforeach()
endif()

add_subdirectory(Sandbox)