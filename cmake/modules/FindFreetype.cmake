include(FindPackageHandleStandardArgs)
include(FetchContent)

set(FT_DISABLE_BROTLI   ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2    ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG      ON CACHE BOOL "" FORCE)
set(FT_DISABLE_ZLIB     ON CACHE BOOL "" FORCE)
set(FT_DISABLE_INSTALL  ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
	freetype
	URL https://download.savannah.gnu.org/releases/freetype/freetype-2.14.3.tar.gz
	URL_HASH SHA256=e61b31ab26358b946e767ed7eb7f4bb2e507da1cfefeb7a8861ace7fd5c899a1
	DOWNLOAD_EXTRACT_TIMESTAMP TRUE
	OVERRIDE_FIND_PACKAGE
	EXCLUDE_FROM_ALL
)

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

set(FREETYPE_FOUND TRUE)
find_package_handle_standard_args(Freetype REQUIRED_VARS FREETYPE_TARGET)