cmake_minimum_required(VERSION 4.0.3)

set(PATH_TO_LIB "${CMAKE_CURRENT_SOURCE_DIR}/..")

project(tests CXX)

list(APPEND CMAKE_PREFIX_PATH ${PATH_TO_LIB})
find_package(Engine REQUIRED)

add_executable(engine_tests engine.cpp)

target_link_libraries(engine_tests PRIVATE Engine::Engine)

target_compile_features(engine_tests PRIVATE cxx_std_17)