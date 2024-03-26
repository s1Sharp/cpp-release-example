cmake_minimum_required(VERSION 3.10)

set(EXECUTABLE_NAME client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -O2")

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME} fancy-cli ${Boost_LIBRARIES} spdlog::spdlog cli::cli)

