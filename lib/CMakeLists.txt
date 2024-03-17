set(
	COMMON_INCLUDE_FILE_LIST 
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/common
)

include_directories(
	${COMMON_INCLUDE_FILE_LIST}
)

set(
	COMMON_SOURCE_FILE_LIST 
		${CMAKE_CURRENT_SOURCE_DIR}/common/utils.cpp
)

add_subdirectory(version)
add_subdirectory(fancycli)
add_subdirectory(fancyser)
