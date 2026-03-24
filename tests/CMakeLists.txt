cmake_minimum_required(VERSION 4.0.3)

set(PATH_TO_LIB "${CMAKE_CURRENT_SOURCE_DIR}/..")

project(tests CXX)

list(APPEND CMAKE_PREFIX_PATH ${PATH_TO_LIB})
find_package(Engine REQUIRED)

add_executable(graphics_tests graphics.cpp)
add_executable(math_tests math.cpp)
add_executable(engine_tests engine.cpp)

target_link_libraries(math_tests PRIVATE Engine::Engine)
target_link_libraries(graphics_tests PRIVATE Engine::Engine)
target_link_libraries(engine_tests PRIVATE Engine::Engine)

target_include_directories(graphics_tests PRIVATE ${PATH_TO_LIB}/deps/glad/include) #for tests
target_include_directories(graphics_tests PRIVATE ${PATH_TO_LIB}/deps/glfw-3.4/include) #for tests

target_compile_features(math_tests PRIVATE cxx_std_17)
target_compile_features(graphics_tests PRIVATE cxx_std_17)
target_compile_features(engine_tests PRIVATE cxx_std_17)