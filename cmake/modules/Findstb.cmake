find_path(STB_INCLUDE_DIR 
    NAMES stb/stb_image.h
    PATHS
        /usr/include
        /usr/local/include
        ${CMAKE_INCLUDE_PATH}
        ${CMAKE_CURRENT_LIST_DIR}/../../deps/stb/include
    REQUIRED
)

if(STB_INCLUDE_DIR)
    set(STB_FOUND TRUE)
else()
    set(STB_FOUND)
    message(FATAL_ERROR "stb not found!")
endif()

mark_as_advanced(STB_INCLUDE_DIR)