set(_glad_root "${CMAKE_CURRENT_LIST_DIR}/../../deps/glad")

find_path(GLAD_INCLUDE_DIR 
	NAMES glad/gl.h
	PATHS "${_glad_root}/include"
	NO_DEFAULT_PATH
)

find_file(GLAD_SOURCE
	NAMES gl.c
	PATHS "${_glad_root}/src"
	NO_DEFAULT_PATH
)

if(NOT GLAD_INCLUDE_DIR OR NOT GLAD_SOURCE)
	set(GLAD_FOUND FALSE)
	message(FATAL_ERROR
		"glad not found under ${_glad_root}. "
		"Ожидались ${_glad_root}/include/glad/gl.h и ${_glad_root}/src/gl.c")
endif()

set(GLAD_FOUND TRUE)

if(NOT TARGET glad::glad)
	add_library(glad_objects OBJECT "${GLAD_SOURCE}")
	target_include_directories(glad_objects PUBLIC "${GLAD_INCLUDE_DIR}")
	target_compile_features(glad_objects PUBLIC c_std_99)
	add_library(glad::glad ALIAS glad_objects)
endif()

mark_as_advanced(GLAD_INCLUDE_DIR GLAD_SOURCE)