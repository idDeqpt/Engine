cmake_minimum_required(VERSION 4.0.3)

set(PATH_TO_LIB "C:/Projects/C++/libraries/Engine/Graphics")
set(PATH_TO_GLAD "C:/Projects/C++/libraries/glad")
set(PATH_TO_GLFW "C:/Projects/C++/libraries/glfw-3.4")


project(tests CXX)


list(APPEND CMAKE_PREFIX_PATH ${PATH_TO_LIB})
find_package(Engine REQUIRED)

add_executable(graphics_tests graphics.cpp)
add_executable(math_tests math.cpp)

target_include_directories(math_tests PRIVATE ${ENGINE_INCLUDE_DIRS})
target_link_libraries(math_tests PRIVATE Engine::Engine)

target_include_directories(graphics_tests PRIVATE ${ENGINE_INCLUDE_DIRS})
target_link_libraries(graphics_tests PRIVATE Engine::Engine)


target_include_directories(graphics_tests PRIVATE ${PATH_TO_GLAD}/include)
target_link_libraries(graphics_tests PRIVATE ${PATH_TO_GLAD}/lib/glad.lib)

target_include_directories(graphics_tests PRIVATE ${PATH_TO_GLFW}/include)
target_link_libraries(graphics_tests PRIVATE ${PATH_TO_GLFW}/lib/glfw3.lib)

target_compile_features(math_tests PRIVATE cxx_std_17)
target_compile_features(graphics_tests PRIVATE cxx_std_17)

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