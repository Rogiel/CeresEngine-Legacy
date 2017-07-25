find_package(VorbisFile)

if(VORBIS_FILE_FOUND)
    list_current_sources_and_subdirs(SRCS DIRS)
    
    add_library(XYZ.Audio.Loader.OggVorbis STATIC ${SRCS})
    target_include_directories(XYZ.Audio.Loader.OggVorbis
            PRIVATE $<TARGET_PROPERTY:XYZ.Engine,INTERFACE_INCLUDE_DIRECTORIES>
            PRIVATE ${VORBIS_FILE_INCLUDE_DIRS}
    )
    target_link_libraries(XYZ.Audio.Loader.OggVorbis
            PUBLIC ${VORBIS_FILE_LIBRARY}
    )
    
    foreach (subdir ${DIRS})
        add_subdirectory(${subdir})
    endforeach ()
endif()