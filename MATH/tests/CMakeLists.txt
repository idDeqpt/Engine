cmake_minimum_required(VERSION 4.0.3)

set(PROJECT_NAME math_lib_test)
set(PATH_TO_MATH "../")

project(${PROJECT_NAME} CXX)

list(APPEND CMAKE_PREFIX_PATH ${PATH_TO_MATH})
find_package(Math REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${MATH_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Engine::Math)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)