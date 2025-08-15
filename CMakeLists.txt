cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME Graphics)
set(BUILD_SHARED_LIBS FALSE)

project(${PROJECT_NAME} CXX)
include_directories("./include")

set(SOURCE_FILES
    src/CanvasItem.cpp
	src/Color.cpp
	src/EventManager.cpp
	src/Font.cpp
	src/GeometricMesh.cpp
	src/LightManager.cpp
	src/Material.cpp
	src/Mesh.cpp
	src/RenderStates.cpp
	src/Shader.cpp
	src/Text2D.cpp
	src/Texture.cpp
	src/View.cpp
	src/Window.cpp)

set(PATH_TO_GLAD "C:/Projects/C++/libraries/glad")
set(PATH_TO_GLFW "C:/Projects/C++/libraries/glfw-3.4")
set(PATH_TO_STB "C:/Projects/C++/libraries/stb")
set(PATH_TO_MATH "C:/Projects/C++/libraries/Engine/Math")
set(PATH_TO_FREETYPE "C:/Projects/C++/libraries/freetype")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_package(stb REQUIRED)
find_package(Math REQUIRED)
find_package(freetype REQUIRED)
find_package(glad REQUIRED)
find_package(glfw REQUIRED)

if (BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${STB_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${MATH_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLAD_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${MATH_LIB_FILE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FREETYPE_LIB_FILE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GLAD_LIB_FILE})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIB_FILE})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

#Установка целевого продукта
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION bin/lib              #Библиотеки будут устанавливаться сюда
    ARCHIVE DESTINATION bin/lib              #То же самое для архивированных версий (.lib)
    RUNTIME DESTINATION bin                  #Исполняемые файлы (*.exe)
    PUBLIC_HEADER DESTINATION include)       #Заголовочные файлы

install(FILES cmake/MathConfig.cmake DESTINATION lib/cmake/${PROJECT_NAME})

#Экспорт целевой спецификации
install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE Engine::          #Пространство имён для целей экспорта
    FILE ${PROJECT_NAME}Config.cmake
    DESTINATION lib/cmake/${PROJECT_NAME})

export(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE Engine::
    FILE ${PROJECT_NAME}Config.cmake)

#Определение версии пакета
set(PACKAGE_VERSION "1.0.0")

#Генерация файла конфигурации с версией
configure_file(
    "cmake/${PROJECT_NAME}ConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    @ONLY)

#Установка файла конфигурации
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION lib/cmake/${PROJECT_NAME})