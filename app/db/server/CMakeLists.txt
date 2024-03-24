cmake_minimum_required(VERSION 3.13)

set(EXECUTABLE_NAME server)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

set_target_properties(
		${EXECUTABLE_NAME}
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
)

find_package(Boost REQUIRED system thread)
find_package(spdlog REQUIRED)

target_link_libraries(${EXECUTABLE_NAME} fancy-ser
	${Boost_LIBRARIES}
	spdlog::spdlog
	redis++::redis++_static
	hiredis::hiredis_static
)
