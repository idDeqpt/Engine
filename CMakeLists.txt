cmake_minimum_required(VERSION 4.0.3)

set(PROJECT_NAME Math)

project(${PROJECT_NAME} CXX)
include_directories("./include")

set(SOURCE_LIB
	)

# Создание динамической библиотеки с именем ${PROJECT_NAME}
add_library(${PROJECT_NAME} SHARED ${SOURCE_LIB})

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