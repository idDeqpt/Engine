cmake_minimum_required(VERSION 3.27)

set(PROJECT_NAME Engine)
set(BUILD_SHARED_LIBS FALSE)

project(${PROJECT_NAME}
	VERSION 1.0.0
	LANGUAGES CXX)

option(BUILD_WITH_OPENGL_3_3 "Build library with support of OpenGL 3.3" ON)
option(BUILD_EXAMPLES        "Build examples for library"               OFF)

set(CORE_SOURCE_FILES
	src/Core/ConfigManager.cpp
	src/Core/Engine.cpp
	src/Core/Logger.cpp
	src/Core/NameTag.cpp
	src/Core/Node.cpp
	src/Core/Node2D.cpp
	src/Core/Node3D.cpp
	src/Core/NodeNameTag.cpp
	src/Core/ResourceManager.cpp
	src/Core/SignalBus.cpp
	src/Core/String.cpp
	src/Core/Timer.cpp
	src/Core/TimeManager.cpp)

set(GRAPHICS_SOURCE_FILES
	src/Graphics/GL/Api.cpp
	src/Graphics/GL/TextureHandle.cpp
	src/Graphics/GL/TexturePool.cpp
	src/Graphics/GL/Null/Api.cpp
	src/Graphics/GL/Null/ArrayBuffer.cpp
	src/Graphics/GL/Null/FrameBuffer.cpp
	src/Graphics/GL/Null/TextureImpl.cpp
	src/Graphics/GL/OpenGL33/Api.cpp
	src/Graphics/GL/OpenGL33/ArrayBuffer.cpp
	src/Graphics/GL/OpenGL33/FrameBuffer.cpp
	src/Graphics/GL/OpenGL33/TextureImpl.cpp
	src/Graphics/2D/Camera2D.cpp
	src/Graphics/2D/CanvasItem.cpp
	src/Graphics/2D/Shape2D.cpp
	src/Graphics/2D/Sprite2D.cpp
	src/Graphics/2D/Text2D.cpp
	src/Graphics/2D/RenderCanvas.cpp
	src/Graphics/3D/Camera3D.cpp
	src/Graphics/3D/GeometricMesh.cpp
	src/Graphics/3D/Mesh.cpp
	src/Graphics/3D/RenderWorld.cpp
	src/Graphics/Color.cpp
	src/Graphics/Font.cpp
	src/Graphics/LightManager.cpp
	src/Graphics/Material.cpp
	src/Graphics/RenderScene.cpp
	src/Graphics/RenderStates.cpp
	src/Graphics/RenderTarget.cpp
	src/Graphics/Shader.cpp
	src/Graphics/Texture.cpp)

set(MATH_SOURCE_FILES
	src/Math/Vec2.cpp
	src/Math/Vec3.cpp
	src/Math/Vec4.cpp
	src/Math/Mat3.cpp
	src/Math/Mat4.cpp
	src/Math/Quaternion.cpp
	src/Math/Transform2.cpp
	src/Math/Transform3.cpp)

set(PHYSICS_SOURCE_FILES
	src/Physics/2D/BVH.cpp
	src/Physics/2D/BVHNode.cpp
	src/Physics/2D/CircleCollider2D.cpp
	src/Physics/2D/Collider2D.cpp
	src/Physics/2D/CollisionData.cpp
	src/Physics/2D/CollisionDetector2D.cpp
	src/Physics/2D/RectangleCollider2D.cpp
	src/Physics/2D/RigidBody2D.cpp
	src/Physics/2D/SimpleCollisionDetectors2D.cpp
	src/Physics/2D/StaticBody2D.cpp
	src/Physics/PhysicsWorld.cpp)

set(SYSTEM_SOURCE_FILES
	src/System/EventManager.cpp
	src/System/Keyboard.cpp
	src/System/Mouse.cpp
	src/System/Window.cpp)


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

set(GLAD_ROOT     "${CMAKE_CURRENT_SOURCE_DIR}/deps/glad"     CACHE PATH "Path to glad")
set(GLFW3_ROOT    "${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw-3.4" CACHE PATH "Path to GLFW3")
set(FREETYPE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/deps/freetype" CACHE PATH "Path to FreeType")

find_package(stb REQUIRED)
find_package(glad REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(freetype REQUIRED)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if(BUILD_WITH_OPENGL_3_3)
	target_compile_definitions(${PROJECT_NAME} PRIVATE USE_OPENGL_3_3)
endif()

target_sources(${PROJECT_NAME} PRIVATE
	${CORE_SOURCE_FILES}
	${GRAPHICS_SOURCE_FILES}
	${MATH_SOURCE_FILES}
	${PHYSICS_SOURCE_FILES}
	${SYSTEM_SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
	PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${STB_INCLUDE_DIR})

set(ENGINE_DEPENDENCIES
	glad::glad
	Freetype::Freetype
	GLFW3::GLFW3)

target_link_libraries(${PROJECT_NAME} PUBLIC ${ENGINE_DEPENDENCIES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(BUILD_EXAMPLES)
	add_subdirectory(examples)

	file(GLOB RESOURCE_DIRS 
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/*/resources"
    )
    
    add_custom_target(CopyAllResources ALL
        COMMENT "Copying resources for all examples"
    )
    
    foreach(RESOURCE_DIR ${RESOURCE_DIRS})
        get_filename_component(EXAMPLE_DIR ${RESOURCE_DIR} DIRECTORY)
        get_filename_component(EXAMPLE_NAME ${EXAMPLE_DIR} NAME)
        
        set(DEST_DIR "${CMAKE_BINARY_DIR}/examples/${EXAMPLE_NAME}/resources")
        
        add_custom_command(TARGET CopyAllResources POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${RESOURCE_DIR}
                ${DEST_DIR}
            COMMENT "Copying resources for ${EXAMPLE_NAME} to ${DEST_DIR}"
        )
    endforeach()
endif()


include(GNUInstallDirs)

#Установка целевого продукта
install(TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}Targets
	LIBRARY DESTINATION bin/lib              #Библиотеки будут устанавливаться сюда
	ARCHIVE DESTINATION bin/lib              #То же самое для архивированных версий (.lib)
	RUNTIME DESTINATION bin                  #Исполняемые файлы (*.exe)
	PUBLIC_HEADER DESTINATION include)       #Заголовочные файлы

#Экспорт целевой спецификации
install(EXPORT ${PROJECT_NAME}Targets
	NAMESPACE Engine::          #Пространство имён для целей экспорта
	FILE ${PROJECT_NAME}Config.cmake
	DESTINATION lib/cmake/${PROJECT_NAME})

export(EXPORT ${PROJECT_NAME}Targets
	NAMESPACE Engine::
	FILE ${PROJECT_NAME}Config.cmake)

#Генерация файла конфигурации с версией
configure_file(
	"cmake/${PROJECT_NAME}ConfigVersion.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	@ONLY)

#Установка файла конфигурации
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
	DESTINATION lib/cmake/${PROJECT_NAME})