cmake_minimum_required(VERSION 4.0.3)

set(PROJECT_NAME graphics_lib_test)
set(PATH_TO_LIB "C:/Projects/C++/libraries/Engine/Graphics")
set(PATH_TO_GLAD "C:/Projects/C++/libraries/glad")
set(PATH_TO_GLFW "C:/Projects/C++/libraries/glfw-3.4")
set(PATH_TO_MATH "C:/Projects/C++/libraries/Engine/Math")
set(PATH_TO_FREETYPE "C:/Projects/C++/libraries/freetype")


project(${PROJECT_NAME} CXX)

add_executable(${PROJECT_NAME} test.cpp)


target_include_directories(${PROJECT_NAME} PRIVATE ${PATH_TO_GLAD}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PATH_TO_GLAD}/lib/glad.lib)

target_include_directories(${PROJECT_NAME} PRIVATE ${PATH_TO_GLFW}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PATH_TO_GLFW}/lib/glfw3.lib)

target_include_directories(${PROJECT_NAME} PRIVATE ${PATH_TO_LIB}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PATH_TO_LIB}/bin/lib/Debug/graphics.lib)

target_include_directories(${PROJECT_NAME} PRIVATE ${PATH_TO_MATH}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PATH_TO_MATH}/bin/lib/Debug/Math.lib)

target_include_directories(${PROJECT_NAME} PRIVATE ${PATH_TO_FREETYPE}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PATH_TO_FREETYPE}/build/Debug/freetyped.lib)

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