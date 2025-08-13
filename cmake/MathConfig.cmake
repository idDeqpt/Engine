if(NOT TARGET Engine::Math)
    add_library(Engine::Math UNKNOWN IMPORTED)

    #Включение заголовочных файлов
    set_target_properties(Engine::Math PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/../include")
    
    #Установка целевого файла библиотеки
    set_target_properties(Engine::Math PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/Math.lib")
endif()