# Locate VORBIS_FILE
# This module defines XXX_FOUND, XXX_INCLUDE_DIRS and XXX_LIBRARIES standard variables
#
# $VORBIS_FILEDIR is an environment variable that would
# correspond to the ./configure --prefix=$VORBIS_FILEDIR
# used in building VORBIS_FILE.

SET(VORBIS_FILE_SEARCH_PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt
)

SET(MSVC_YEAR_NAME)
IF (MSVC_VERSION GREATER 1599)		# >= 1600
    SET(MSVC_YEAR_NAME VS2010)
ELSEIF(MSVC_VERSION GREATER 1499)	# >= 1500
    SET(MSVC_YEAR_NAME VS2008)
ELSEIF(MSVC_VERSION GREATER 1399)	# >= 1400
    SET(MSVC_YEAR_NAME VS2005)
ELSEIF(MSVC_VERSION GREATER 1299)	# >= 1300
    SET(MSVC_YEAR_NAME VS2003)
ELSEIF(MSVC_VERSION GREATER 1199)	# >= 1200
    SET(MSVC_YEAR_NAME VS6)
ENDIF()

FIND_PATH(VORBIS_FILE_INCLUDE_DIR
        NAMES vorbis/vorbisfile.h
        HINTS ENV VORBIS_FILEDIR
        PATH_SUFFIXES include
        PATHS ${VORBIS_FILE_SEARCH_PATHS}
)

FIND_LIBRARY(VORBIS_FILE_LIBRARY
        NAMES vorbisfile libvorbisfile
        HINTS ENV VORBIS_FILEDIR
        PATH_SUFFIXES lib lib64 win32/Dynamic_Release "Win32/${MSVC_YEAR_NAME}/x64/Release" "Win32/${MSVC_YEAR_NAME}/Win32/Release"
        PATHS ${VORBIS_FILE_SEARCH_PATHS}
)

# First search for d-suffixed libs
FIND_LIBRARY(VORBIS_FILE_LIBRARY_DEBUG
        NAMES vorbisfiled vorbisfile_d libvorbisfiled libvorbisfile_d
        HINTS ENV VORBIS_FILEDIR
        PATH_SUFFIXES lib lib64 win32/Dynamic_Debug "Win32/${MSVC_YEAR_NAME}/x64/Debug" "Win32/${MSVC_YEAR_NAME}/Win32/Debug"
        PATHS ${VORBIS_FILE_SEARCH_PATHS}
)

IF(NOT VORBIS_FILE_LIBRARY_DEBUG)
    # Then search for non suffixed libs if necessary, but only in debug dirs
    FIND_LIBRARY(VORBIS_FILE_LIBRARY_DEBUG
            NAMES vorbisfile libvorbisfile
            HINTS ENV VORBIS_FILEDIR
            PATH_SUFFIXES win32/Dynamic_Debug "Win32/${MSVC_YEAR_NAME}/x64/Debug" "Win32/${MSVC_YEAR_NAME}/Win32/Debug"
            PATHS ${VORBIS_FILE_SEARCH_PATHS}
    )
ENDIF()


IF(VORBIS_FILE_LIBRARY)
    IF(VORBIS_FILE_LIBRARY_DEBUG)
        SET(VORBIS_FILE_LIBRARIES optimized "${VORBIS_FILE_LIBRARY}" debug "${VORBIS_FILE_LIBRARY_DEBUG}")
    ELSE()
        SET(VORBIS_FILE_LIBRARIES "${VORBIS_FILE_LIBRARY}")		# Could add "general" keyword, but it is optional
    ENDIF()
ENDIF()

# handle the QUIETLY and REQUIRED arguments and set XXX_FOUND to TRUE if all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(VORBIS_FILE DEFAULT_MSG VORBIS_FILE_LIBRARIES VORBIS_FILE_INCLUDE_DIR)