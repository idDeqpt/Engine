if(NOT TARGET Engine::Graphics)
    add_library(Engine::Graphics UNKNOWN IMPORTED)

    #Подключение папки зависимостей
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules" ${CMAKE_MODULE_PATH})

    #Поиск библиотек
    find_package(Math REQUIRED)
    find_package(freetype REQUIRED)

    #Включение заголовочных файлов
    set_target_properties(Engine::Graphics PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/../include;${MATH_INCLUDE_DIR}")
    
    #Установка целевого файла библиотеки
    set_target_properties(Engine::Graphics PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/Graphics.lib")

    #Передача зависимостей на экспорт
    set_target_properties(Engine::Graphics PROPERTIES
        INTERFACE_LINK_LIBRARIES "${MATH_LIB_FILE};${FREETYPE_LIB_FILE}")
endif()