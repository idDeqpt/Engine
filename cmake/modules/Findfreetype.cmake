include(FindPackageHandleStandardArgs)

find_path(FREETYPE_INCLUDE_DIR
    NAMES ft2build.h
    PATHS
        /usr/include
        /usr/local/include
        /usr/include/freetype2
        /usr/local/include/freetype2
        ${CMAKE_INCLUDE_PATH}
        ${PROJECT_SOURCE_DIR}/third_party/freetype/include
        ${PROJECT_SOURCE_DIR}/third_party/freetype/include/freetype2
)

if(FREETYPE_INCLUDE_DIR)
    if(EXISTS "${FREETYPE_INCLUDE_DIR}/freetype/freetype.h")
        set(FREETYPE_INCLUDE_DIRS ${FREETYPE_INCLUDE_DIR})
    elseif(EXISTS "${FREETYPE_INCLUDE_DIR}/freetype2/freetype/freetype.h")
        set(FREETYPE_INCLUDE_DIRS ${FREETYPE_INCLUDE_DIR}/freetype2)
    else()
        find_path(FREETYPE_INCLUDE_DIR_freetype2
            NAMES freetype/freetype.h
            PATHS
                /usr/include/freetype2
                /usr/local/include/freetype2
                ${PROJECT_SOURCE_DIR}/third_party/freetype/include/freetype2
        )
        if(FREETYPE_INCLUDE_DIR_freetype2)
            set(FREETYPE_INCLUDE_DIRS ${FREETYPE_INCLUDE_DIR_freetype2})
        endif()
    endif()
endif()

find_library(FREETYPE_LIBRARY
    NAMES freetype libfreetype
    PATHS
        /usr/lib
        /usr/local/lib
        ${CMAKE_LIBRARY_PATH}
        ${PROJECT_SOURCE_DIR}/third_party/freetype/lib
        ${PROJECT_SOURCE_DIR}/third_party/freetype/build
)

if(NOT FREETYPE_INCLUDE_DIR OR NOT FREETYPE_LIBRARY)
    message(STATUS "FreeType not found in system, will download and build via FetchContent")
    
    include(FetchContent)
    
    FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
        GIT_TAG VER-2-14-3
    )
    
    set(FREETYPE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(FREETYPE_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(freetype)
    
    if(TARGET freetype)
        set(FREETYPE_TARGET freetype)
    else()
        message(FATAL_ERROR "FreeType target not found after FetchContent")
    endif()
    
    if(NOT TARGET Freetype::Freetype)
        add_library(Freetype::Freetype INTERFACE IMPORTED)
        target_link_libraries(Freetype::Freetype INTERFACE ${FREETYPE_TARGET})
        target_include_directories(Freetype::Freetype INTERFACE 
            $<TARGET_PROPERTY:${FREETYPE_TARGET},INTERFACE_INCLUDE_DIRECTORIES>
        )
    endif()
    
    get_target_property(FREETYPE_INCLUDE_DIR ${FREETYPE_TARGET} INTERFACE_INCLUDE_DIRECTORIES)
    set(FREETYPE_LIBRARY ${FREETYPE_TARGET})
    set(FREETYPE_FOUND TRUE)
    
else()
    if(NOT TARGET Freetype::Freetype)
        add_library(Freetype::Freetype INTERFACE IMPORTED)
        target_include_directories(Freetype::Freetype INTERFACE ${FREETYPE_INCLUDE_DIRS})
        target_link_libraries(Freetype::Freetype INTERFACE ${FREETYPE_LIBRARY})
    endif()
    
    set(FREETYPE_FOUND TRUE)
endif()

find_package_handle_standard_args(Freetype
    REQUIRED_VARS FREETYPE_INCLUDE_DIR FREETYPE_LIBRARY
    FOUND_VAR FREETYPE_FOUND
)

mark_as_advanced(FREETYPE_INCLUDE_DIR FREETYPE_LIBRARY)