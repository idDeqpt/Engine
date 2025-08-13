cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME Math)
set(BUILD_SHARED_LIBS FALSE)

project(${PROJECT_NAME} CXX)
include_directories("./include")

set(SOURCE_FILES
    src/Vec2.cpp
    src/Vec3.cpp
    src/Vec4.cpp
    src/Mat3.cpp
    src/Mat4.cpp
    src/Quaternion.cpp
    src/Transform2.cpp
    src/Transform3.cpp
    src/Transformable2.cpp
    src/Transformable3.cpp)

if (BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()

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

#Определение версии пакета
set(PACKAGE_VERSION "1.0.0")

#Генерация файла конфигурации с версией
configure_file(
    "cmake/MathConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MathConfigVersion.cmake"
    @ONLY
)

#Установка файла конфигурации
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MathConfigVersion.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
)