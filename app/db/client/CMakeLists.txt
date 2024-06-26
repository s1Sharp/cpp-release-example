cmake_minimum_required(VERSION 3.10)

set(EXECUTABLE_NAME fancy_client)


set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${EXECUTABLE_NAME}
	${SOURCES}
)

set_target_properties(
		${EXECUTABLE_NAME}
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
)

target_include_directories(${EXECUTABLE_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)

if(BUILD_SHARED_LIB)
	target_link_libraries(${EXECUTABLE_NAME} 
		PRIVATE
			fancy-cli
			${Boost_LIBRARIES}
			spdlog::spdlog
			cli::cli
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
			fancy-cli
			${Boost_LIBRARIES}
			spdlog::spdlog
			cli::cli
	)
endif()
