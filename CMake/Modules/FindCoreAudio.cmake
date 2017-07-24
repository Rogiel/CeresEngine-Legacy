# -----------------------------------------------------------------------------
# Find CoreAudio framework (Mac OS X).
#
# Define:
# CoreAudio_FOUND
# CoreAudio_INCLUDE_DIR
# CoreAudio_LIBRARY

set(CoreAudio_FOUND false)
set(CoreAudio_INCLUDE_DIR)
set(CoreAudio_LIBRARY)

if(APPLE)
    # The only platform it makes sense to check for CoreAudio
    find_library(CoreAudio CoreAudio)
    if(CoreAudio)
        set(CoreAudio_FOUND true)
        set(CoreAudio_INCLUDE_DIR ${CoreAudio})
        set(CoreAudio_LIBRARY ${CoreAudio})

        find_package_handle_standard_args(CoreAudio DEFAULT_MSG CoreAudio_INCLUDE_DIR CoreAudio_LIBRARY)
    endif()
endif()