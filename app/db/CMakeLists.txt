include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/common
)

set(
	COMMON_SOURCE_FILE_LIST 
		${CMAKE_CURRENT_SOURCE_DIR}/common/utils.cpp
)

add_subdirectory(server)
add_subdirectory(client)
