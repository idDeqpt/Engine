# Поиск заголовочных файлов FreeType
find_path(GLAD_INCLUDE_DIR
    NAMES glad/glad.h
    PATHS
        ${GLAD_ROOT}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/glad/include
    REQUIRED
    NO_DEFAULT_PATH
    DOC "Path to glad include directory"
)

# Поиск библиотеки FreeType
find_library(GLAD_LIBRARY
    NAMES glad
    PATHS
        ${GLAD_ROOT}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/glad/lib
    REQUIRED
    NO_DEFAULT_PATH
    DOC "Path to glad library"
)

# Обработка результата
if(GLAD_INCLUDE_DIR AND GLAD_LIBRARY)
    set(GLAD_FOUND TRUE)

    if(NOT TARGET glad::glad)
        add_library(glad::glad UNKNOWN IMPORTED)
        set_target_properties(glad::glad PROPERTIES
            IMPORTED_LOCATION "${GLAD_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${GLAD_INCLUDE_DIR}"
        )
    endif()
    message(STATUS "Found glad: SUCCESS")
else()
    set(GLAD_FOUND FALSE)
    message(FATAL_ERROR
        "glad not found!\n"
        "Expected structure:\n"
        "  ${GLAD_ROOT}/include/glad/glad.h\n"
        "  ${GLAD_ROOT}/lib/glad.lib (or libglad.a, etc.)"
    )
endif()

mark_as_advanced(GLAD_INCLUDE_DIR GLAD_LIBRARY)