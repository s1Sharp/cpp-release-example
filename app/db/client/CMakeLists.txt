cmake_minimum_required(VERSION 3.13)

set(EXECUTABLE_NAME async_client)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost COMPONENTS log program_options system REQUIRED)
find_package(Threads)

add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_client.cpp)

target_link_libraries(${EXECUTABLE_NAME}
    ${Boost_LIBRARIES})

install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
