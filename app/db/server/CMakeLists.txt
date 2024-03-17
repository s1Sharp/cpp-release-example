cmake_minimum_required(VERSION 3.13)

set(EXECUTABLE_NAME async_server)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost COMPONENTS REQUIRED
    log
    program_options
    system)
find_package(Threads)

add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/acceptor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/service.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server_base.cpp
)

target_link_libraries(${EXECUTABLE_NAME}
    ${Boost_LIBRARIES})

install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
