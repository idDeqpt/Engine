cmake_minimum_required(VERSION 3.27)

set(PROJECT_NAME Simple3D)

project(${PROJECT_NAME} CXX)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE Engine::Engine)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)