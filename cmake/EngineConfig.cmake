if(NOT TARGET Engine::Engine)
    add_library(Engine::Engine UNKNOWN IMPORTED)

    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules" ${CMAKE_MODULE_PATH})

    set(ENGINE_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")

    find_package(glad REQUIRED)
    find_package(GLFW3 REQUIRED)
    find_package(freetype REQUIRED)

	if(WIN32)
		set(ENGINE_LIB_NAME "Engine.lib")
	else()
		set(ENGINE_LIB_NAME "libEngine.a")
	endif()
    
    set_target_properties(Engine::Engine PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/../include"
        INTERFACE_LINK_LIBRARIES "${FREETYPE_LIBRARY};${GLFW3_LIBRARY};${GLAD_LIBRARY}")
	
	if(NOT CMAKE_CONFIGURATION_TYPES)
        set_target_properties(Engine::Engine PROPERTIES
            IMPORTED_LOCATION "${ENGINE_ROOT}/build/${ENGINE_LIB_NAME}")
    else()
        set_target_properties(Engine::Engine PROPERTIES
            IMPORTED_LOCATION_DEBUG   "${ENGINE_ROOT}/build/Debug/${ENGINE_LIB_NAME}"
            IMPORTED_LOCATION_RELEASE "${ENGINE_ROOT}/build/Release/${ENGINE_LIB_NAME}"
			IMPORTED_LOCATION_RELWITHDEBINFO "${ENGINE_ROOT}/build/Release/${ENGINE_LIB_NAME}"
			IMPORTED_LOCATION_MINSIZEREL     "${ENGINE_ROOT}/build/Release/${ENGINE_LIB_NAME}"
			MAP_IMPORTED_CONFIG_RELWITHDEBINFO "RELEASE"
			MAP_IMPORTED_CONFIG_MINSIZEREL     "RELEASE")
    endif()
endif()