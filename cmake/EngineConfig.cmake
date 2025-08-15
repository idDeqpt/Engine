if(NOT TARGET Engine::Engine)
    add_library(Engine::Engine UNKNOWN IMPORTED)

    #Подключение папки зависимостей
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules" ${CMAKE_MODULE_PATH})

    #Поиск библиотек
    find_package(freetype REQUIRED)

    #Включение заголовочных файлов
    set_target_properties(Engine::Engine PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/../include")
    
    #Установка целевого файла библиотеки
    set_target_properties(Engine::Engine PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/Engine.lib")

    #Передача зависимостей на экспорт
    set_target_properties(Engine::Engine PROPERTIES
        INTERFACE_LINK_LIBRARIES "${FREETYPE_LIB_FILE}")
endif()