file(GLOB EMAMPLE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/
	 ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(ex_dir ${EMAMPLE_DIRS})
	if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${ex_dir})
		add_subdirectory(${ex_dir})
	endif()
endforeach()