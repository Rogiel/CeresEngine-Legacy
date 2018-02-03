#
# Try to find Blender executable
# Once done this will define
#
# Blender_FOUND       - TRUE if Blender executable is found
# Blender_EXECUTABLE  - Full path to Blender executable
#

SET (_BLENDER_HINTS
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\BlenderFoundation;Install_Dir]"
)

SET (_BLENDER_PATHS
        /usr/bin
        /usr/local/bin
        /bin
        /Applications/Blender.app
)

FIND_PROGRAM(Blender_EXECUTABLE
        NAMES blender
        HINTS ${_BLENDER_HINTS}
        PATHS ${_BLENDER_PATHS}
)

include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (Blender DEFAULT_MSG Blender_EXECUTABLE )

MARK_AS_ADVANCED(
        Blender_EXECUTABLE
)