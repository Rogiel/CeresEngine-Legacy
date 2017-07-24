# Next lines needed for building all Qt projects
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

file(GLOB_RECURSE SRCS Source/*.cpp Source/*.hpp)
file(GLOB_RECURSE UIS Source/*.ui)
file(GLOB_RECURSE QRCS_SRC Source/*.qrc)

# Generate code from ui files
qt5_wrap_ui(UI_HEADERS ${UIS})

# Generate rules for building source files from the resources
qt5_add_resources(QRCS ${QRCS_SRC})

add_executable(XYZ.WorldEditor ${SRCS} ${UI_HEADERS} ${QRCS})
target_include_directories(XYZ.WorldEditor
        PUBLIC ${SRC_DIR}
        PUBLIC ${GLM_INCLUDE_DIRS}
)
target_link_libraries(XYZ.WorldEditor
        PRIVATE Qt5::Widgets
)

if (APPLE)
    set_target_properties(XYZ.WorldEditor PROPERTIES
            MACOSX_BUNDLE YES
    )
endif ()
