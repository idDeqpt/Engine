# Поиск заголовочных файлов FreeType
find_path(FREETYPE_INCLUDE_DIR
    NAMES ft2build.h
    PATHS
        ${FREETYPE_ROOT}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/freetype/include
    REQUIRED
    NO_DEFAULT_PATH
    DOC "Path to FreeType include directory"
)

# Поиск библиотеки FreeType
find_library(FREETYPE_LIBRARY
    NAMES freetyped libfreetype freetype2
    PATHS
        ${FREETYPE_ROOT}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/freetype/lib
    REQUIRED
    NO_DEFAULT_PATH
    DOC "Path to FreeType library"
)

# Обработка результата
if(FREETYPE_INCLUDE_DIR AND FREETYPE_LIBRARY)
    set(FREETYPE_FOUND TRUE)

    if(NOT TARGET FreeType::FreeType)
        add_library(FreeType::FreeType UNKNOWN IMPORTED)
        set_target_properties(FreeType::FreeType PROPERTIES
            IMPORTED_LOCATION "${FREETYPE_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${FREETYPE_INCLUDE_DIR}"
        )
    endif()
    message(STATUS "Found FreeType: SUCCESS")
else()
    set(FREETYPE_FOUND FALSE)
    message(FATAL_ERROR
        "FreeType not found!\n"
        "Expected structure:\n"
        "  ${FREETYPE_ROOT}/include/ft2build.h\n"
        "  ${FREETYPE_ROOT}/lib/freetype.lib (or libfreetype.a, etc.)"
    )
endif()

mark_as_advanced(FREETYPE_INCLUDE_DIR FREETYPE_LIBRARY)