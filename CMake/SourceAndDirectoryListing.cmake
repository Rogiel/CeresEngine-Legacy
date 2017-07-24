macro(_subdirlist result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            list(APPEND dirlist ${curdir}/${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

macro(list_sources_and_subdirs sources subdirs dir)
    file(GLOB _srcs
            ${dir}/*.cpp
            ${dir}/*.hpp
    )
    list(APPEND ${sources} ${_srcs})

    _subdirlist(_SUBDIRS ${dir})
    foreach(_subdir ${_SUBDIRS})
        if(NOT EXISTS ${_subdir}/CMakeLists.txt)
            file(GLOB _srcs
                    ${_subdir}/*.cpp
                    ${_subdir}/*.hpp
            )
            list(APPEND ${sources} ${_srcs})
            list_sources_and_subdirs(${sources} ${subdirs} ${_subdir})
        else()
            list(APPEND ${subdirs} ${_subdir})
        endif()
    endforeach()
endmacro()

macro(list_current_sources_and_subdirs sources subdirs)
    set(${sources} "")
    set(${subdirs} "")
    list_sources_and_subdirs(${sources} ${subdirs} ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()
