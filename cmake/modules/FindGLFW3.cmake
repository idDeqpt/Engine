# Поиск заголовочных файлов
find_path(GLFW3_INCLUDE_DIR
    NAMES GLFW/glfw3.h
    PATHS
        ${GLFW3_ROOT}/include #deps/glfw3/include
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw3/include
    REQUIRED
    NO_DEFAULT_PATH
    DOC "Path to GLFW3 include directory"
)

# Поиск библиотеки
find_library(GLFW3_LIBRARY
    NAMES glfw3 libglfw3 glfw glfw3_mt
    PATHS
        ${GLFW3_ROOT}/lib #deps/glfw3/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw3/lib
    REQUIRED
    NO_DEFAULT_PATH
    DOC "Path to GLFW3 library"
)

# Обработка результата
if(GLFW3_INCLUDE_DIR AND GLFW3_LIBRARY)
    set(GLFW3_FOUND TRUE)
    
    if(NOT TARGET GLFW3::GLFW3)
        add_library(GLFW3::GLFW3 UNKNOWN IMPORTED)
        set_target_properties(GLFW3::GLFW3 PROPERTIES
            IMPORTED_LOCATION "${GLFW3_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${GLFW3_INCLUDE_DIR}"
        )
    endif()
    message(STATUS "Found GLFW3: SUCCESS")
else()
    set(GLFW3_FOUND FALSE)
    message(FATAL_ERROR 
        "GLFW3 not found!\n"
        "Expected structure:\n"
        "  ${GLFW3_ROOT}/include/GLFW/glfw3.h\n"
        "  ${GLFW3_ROOT}/lib/glfw3.lib (or libglfw3.a, etc.)"
    )
endif()

mark_as_advanced(GLFW3_INCLUDE_DIR GLFW3_LIBRARY)