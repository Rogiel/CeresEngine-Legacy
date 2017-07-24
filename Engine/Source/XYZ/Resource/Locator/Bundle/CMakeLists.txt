find_package(CoreFoundation)

if(CoreFoundation_FOUND)
    list_current_sources_and_subdirs(SRCS DIRS)
    
    add_library(XYZ.Resource.Locator.Bundle STATIC ${SRCS})
    target_include_directories(XYZ.Resource.Locator.Bundle
            PRIVATE $<TARGET_PROPERTY:XYZ.Engine,INTERFACE_INCLUDE_DIRECTORIES>
    )
    
    target_link_libraries(XYZ.Resource.Locator.Bundle
            ${CoreFoundation_LIBRARY}
    )
    
    target_compile_definitions(XYZ.Resource.Locator.Bundle
            PUBLIC XYZ_RESOURCE_LOCATOR_BUNDLE=1
    )
    
    foreach (subdir ${DIRS})
        add_subdirectory(${subdir})
    endforeach ()
endif()