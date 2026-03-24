if(NOT TARGET Engine::Engine)
    add_library(Engine::Engine UNKNOWN IMPORTED)

    #Подключение папки зависимостей
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules" ${CMAKE_MODULE_PATH})

    set(ENGINE_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
    set(GLAD_ROOT     "${ENGINE_ROOT}/deps/glad"     CACHE PATH "Path to glad")
    set(GLFW3_ROOT    "${ENGINE_ROOT}/deps/glfw-3.4" CACHE PATH "Path to GLFW3")
    set(FREETYPE_ROOT "${ENGINE_ROOT}/deps/freetype" CACHE PATH "Path to FreeType")

    #Поиск библиотек
    find_package(glad REQUIRED)
    find_package(GLFW3 REQUIRED)
    find_package(FreeType REQUIRED)

    #Включение заголовочных файлов
    set_target_properties(Engine::Engine PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/../include")
    
    #Установка целевого файла библиотеки
    set_target_properties(Engine::Engine PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/Engine.lib")

    #Передача зависимостей на экспорт
    set_target_properties(Engine::Engine PROPERTIES
        INTERFACE_LINK_LIBRARIES "${FREETYPE_LIBRARY};${GLFW3_LIBRARY};${GLAD_LIBRARY}")
endif()