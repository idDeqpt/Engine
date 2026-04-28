cmake_minimum_required(VERSION 4.0.3)

set(PROJECT_NAME app)

project(${PROJECT_NAME} CXX)

set(PATH_TO_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../..")

list(APPEND CMAKE_PREFIX_PATH ${PATH_TO_LIB})
find_package(Engine REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE Engine::Engine)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)