cmake_minimum_required(VERSION 4.0.3)

set(PROJECT_NAME Math)
set(BUILD_SHARED_LIBS FALSE)

project(${PROJECT_NAME} CXX)
include_directories("./include")


set(SOURCE_FILES
	src/Vec2.cpp
	src/Vec3.cpp
	src/Vec4.cpp
	src/Mat3.cpp
	src/Mat4.cpp
	src/Transform3.cpp
	src/Transformable3.cpp)


if (BUILD_SHARED_LIBS)
	set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else()
	add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set(dir ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir})
set(CMAKE_BUILD_FILES_DIRECTORY ${dir})
set(CMAKE_BUILD_DIRECTORY ${dir})
set(CMAKE_BINARY_DIR  ${dir})
SET(EXECUTABLE_OUTPUT_PATH ${dir})
SET(LIBRARY_OUTPUT_PATH ${dir}lib)
SET(CMAKE_CACHEFILE_DIR ${dir})