cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME Engine)
set(BUILD_SHARED_LIBS FALSE)

project(${PROJECT_NAME} CXX)

set(GRAPHICS_SOURCE_FILES
    src/Graphics/CanvasItem.cpp
	src/Graphics/Color.cpp
	src/Graphics/EventManager.cpp
	src/Graphics/Font.cpp
	src/Graphics/GeometricMesh.cpp
	src/Graphics/LightManager.cpp
	src/Graphics/Material.cpp
	src/Graphics/Mesh.cpp
	src/Graphics/RenderStates.cpp
    src/Graphics/Shader.cpp
	src/Graphics/Shape2D.cpp
	src/Graphics/Text2D.cpp
	src/Graphics/Texture.cpp
	src/Graphics/View.cpp
	src/Graphics/Window.cpp)

set(MATH_SOURCE_FILES
    src/Math/Vec2.cpp
    src/Math/Vec3.cpp
    src/Math/Vec4.cpp
    src/Math/Mat3.cpp
    src/Math/Mat4.cpp
    src/Math/Quaternion.cpp
    src/Math/Transform2.cpp
    src/Math/Transform3.cpp
    src/Math/Transformable2.cpp
    src/Math/Transformable3.cpp)


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_package(stb REQUIRED)
find_package(freetype REQUIRED)
find_package(glad REQUIRED)
find_package(glfw REQUIRED)

if (BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_library(${PROJECT_NAME} SHARED )
else()
    add_library(${PROJECT_NAME} STATIC ${GRAPHICS_SOURCE_FILES} ${MATH_SOURCE_FILES})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLAD_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIR})

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