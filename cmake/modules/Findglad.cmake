find_path(GLAD_INCLUDE_DIR 
    NAMES glad/glad.h
    PATHS
        /usr/include
        /usr/local/include
        ${CMAKE_INCLUDE_PATH}
        ${PROJECT_SOURCE_DIR}/deps/glad/include
    REQUIRED
)

find_library(GLAD_LIBRARY
    NAMES glad libglad
    PATHS
        /usr/lib
        /usr/local/lib
        ${CMAKE_LIBRARY_PATH}
        ${PROJECT_SOURCE_DIR}/deps/glad/lib
    REQUIRED
)

if(GLAD_INCLUDE_DIR AND GLAD_LIBRARY)
    set(GLAD_FOUND TRUE)

    if(NOT TARGET glad::glad)
        add_library(glad::glad UNKNOWN IMPORTED)
        set_target_properties(glad::glad PROPERTIES
            IMPORTED_LOCATION "${GLAD_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${GLAD_INCLUDE_DIR}"
        )
    endif()
else()
    set(GLAD_FOUND FALSE)
    message(FATAL_ERROR "glad not found!")
endif()

mark_as_advanced(GLAD_INCLUDE_DIR GLAD_LIBRARY)