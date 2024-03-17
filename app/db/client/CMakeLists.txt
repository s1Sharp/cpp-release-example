cmake_minimum_required(VERSION 3.10)

set(EXECUTABLE_NAME client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -O3")

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

find_package(Boost REQUIRED system thread)
find_package(spdlog REQUIRED)

target_link_libraries(${EXECUTABLE_NAME} fancy-cli ${Boost_LIBRARIES} spdlog::spdlog)

