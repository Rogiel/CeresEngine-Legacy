find_package(CoreAudio)

if(CoreAudio_FOUND)
    list_current_sources_and_subdirs(SRCS DIRS)
    
    add_library(XYZ.Audio.CoreAudio STATIC ${SRCS})
    target_include_directories(XYZ.Audio.CoreAudio
            PRIVATE $<TARGET_PROPERTY:XYZ.Engine,INTERFACE_INCLUDE_DIRECTORIES>
            PRIVATE ${CoreAudio_INCLUDE_DIRS}
    )
    target_link_libraries(XYZ.Audio.CoreAudio
            PRIVATE ${CoreAudio_LIBRARIES}
    )
    
    foreach (subdir ${DIRS})
        add_subdirectory(${subdir})
    endforeach ()
endif()
