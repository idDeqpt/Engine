include(FindPackageHandleStandardArgs)
include(FetchContent)

set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED   OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
	glfw
	URL https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz
	URL_HASH SHA256=c038d34200234d071fae9345bc455e4a8f2f544ab60150765d7704e08f3dac01
	DOWNLOAD_EXTRACT_TIMESTAMP TRUE
	OVERRIDE_FIND_PACKAGE
	EXCLUDE_FROM_ALL
)

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

set(GLFW3_FOUND TRUE)
find_package_handle_standard_args(GLFW3 REQUIRED_VARS GLFW3_TARGET)