find_package(OpenAL)

if(OpenAL_FOUND)
    list_current_sources_and_subdirs(SRCS DIRS)
    
    add_library(XYZ.Audio.OpenAL STATIC ${SRCS})
    target_include_directories(XYZ.Audio.OpenAL
            PRIVATE $<TARGET_PROPERTY:XYZ.Engine,INTERFACE_INCLUDE_DIRECTORIES>
            PRIVATE ${OPENAL_INCLUDE_DIRS}
    )
    target_link_libraries(XYZ.Audio.OpenAL
            PUBLIC ${OPENAL_LIBRARY}
    )
    
    foreach (subdir ${DIRS})
        add_subdirectory(${subdir})
    endforeach ()
endif()