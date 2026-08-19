include(FindPackageHandleStandardArgs)

find_path(GLFW3_INCLUDE_DIR 
    NAMES GLFW/glfw3.h
    PATHS
        /usr/include
        /usr/local/include
        ${CMAKE_INCLUDE_PATH}
        ${PROJECT_SOURCE_DIR}/third_party/glfw/include
)

find_library(GLFW3_LIBRARY
    NAMES glfw3 glfw libglfw3 libglfw
    PATHS
        /usr/lib
        /usr/local/lib
        ${CMAKE_LIBRARY_PATH}
        ${PROJECT_SOURCE_DIR}/third_party/glfw/lib
        ${PROJECT_SOURCE_DIR}/third_party/glfw/build/src
)

if(NOT GLFW3_INCLUDE_DIR OR NOT GLFW3_LIBRARY)
    message(STATUS "GLFW3 not found in system, will download and build via FetchContent")
    
    include(FetchContent)
    
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
    )

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(glfw)
    
    if(TARGET glfw)
        set(GLFW3_TARGET glfw)
    elseif(TARGET glfw3)
        set(GLFW3_TARGET glfw3)
    else()
        message(FATAL_ERROR "GLFW target not found after FetchContent")
    endif()
    
    if(NOT TARGET GLFW3::GLFW3)
        add_library(GLFW3::GLFW3 INTERFACE IMPORTED)
        target_link_libraries(GLFW3::GLFW3 INTERFACE ${GLFW3_TARGET})
        target_include_directories(GLFW3::GLFW3 INTERFACE 
            $<TARGET_PROPERTY:${GLFW3_TARGET},INTERFACE_INCLUDE_DIRECTORIES>
        )
    endif()
    
    get_target_property(GLFW3_INCLUDE_DIR ${GLFW3_TARGET} INTERFACE_INCLUDE_DIRECTORIES)
    set(GLFW3_LIBRARY ${GLFW3_TARGET})
    set(GLFW3_FOUND TRUE)
    
else()
    if(NOT TARGET GLFW3::GLFW3)
        add_library(GLFW3::GLFW3 INTERFACE IMPORTED)
        target_include_directories(GLFW3::GLFW3 INTERFACE ${GLFW3_INCLUDE_DIR})
        target_link_libraries(GLFW3::GLFW3 INTERFACE ${GLFW3_LIBRARY})
    endif()
    
    set(GLFW3_FOUND TRUE)
endif()

if(GLFW3_FOUND)
    if(WIN32)
        target_link_libraries(GLFW3::GLFW3 INTERFACE 
            opengl32
            ${GLFW3_LIBRARY}
        )
    elseif(APPLE)
        find_library(COCOA_LIBRARY Cocoa)
        find_library(IOKIT_LIBRARY IOKit)
        find_library(COREVIDEO_LIBRARY CoreVideo)
        
        target_link_libraries(GLFW3::GLFW3 INTERFACE
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
        )
    else()
        find_package(Threads REQUIRED)
        target_link_libraries(GLFW3::GLFW3 INTERFACE
            ${CMAKE_DL_LIBS}
            Threads::Threads
        )
    endif()
endif()

find_package_handle_standard_args(GLFW3
    REQUIRED_VARS GLFW3_INCLUDE_DIR GLFW3_LIBRARY
    FOUND_VAR GLFW3_FOUND
)

mark_as_advanced(GLFW3_INCLUDE_DIR GLFW3_LIBRARY)