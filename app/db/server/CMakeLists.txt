cmake_minimum_required(VERSION 3.13)

set(EXECUTABLE_NAME fancy_server)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${EXECUTABLE_NAME}
	${SOURCES}
)

target_include_directories(${EXECUTABLE_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(
		${EXECUTABLE_NAME}
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
)

if(BUILD_SHARED_LIB)
	target_link_libraries(${EXECUTABLE_NAME}
		PRIVATE
			fancy-ser
			${Boost_LIBRARIES}
			spdlog::spdlog
			redis++::redis++
			hiredis::hiredis
	)
else()
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_SEARCH_START_STATIC ON)
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_SEARCH_END_STATIC ON)

	target_link_options(
		${EXECUTABLE_NAME}
	PRIVATE
		-static-libgcc
		-static-libstdc++
		-lc
		-static
	)

	target_link_libraries(${EXECUTABLE_NAME}
		PRIVATE
			fancy-ser
			${Boost_LIBRARIES}
			spdlog::spdlog
			redis++::redis++_static
			hiredis::hiredis_static
	)
endif()
