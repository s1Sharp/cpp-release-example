set(LIBRARY_NAME fancy-network)

# Sources are added with target_source(). When doing that, no sources are
# required here.

# Сборка и установка библиотеки
if(BUILD_SHARED_LIB)
	add_library(${LIBRARY_NAME} SHARED "")
else()
	add_library(${LIBRARY_NAME} STATIC "")
endif()

set_target_properties(
		${LIBRARY_NAME}
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
)

# Add headers to private sources for better support in IDEs.
# Исходные файлы библиотеки
target_sources(
		${LIBRARY_NAME}
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src/TcpClient.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/TcpConnection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/TcpServer.cpp
)

target_include_directories(
		${LIBRARY_NAME}
	PRIVATE
		# where the library itself will look for its internal headers
		${CMAKE_CURRENT_SOURCE_DIR}/src
	PUBLIC
		# where top-level project will look for the library's public headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		# where external projects will look for the library's public headers
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


### this one no need to include, we do application delivery, not shared libraries

# # without it public headers won't get installed
# set(public_headers
#	 include/fancy_version.h
# )

# # note that ${public_headers} should be in quotes
# set_target_properties(
#		 ${LIBRARY_NAME} PROPERTIES
#			 PUBLIC_HEADER "${public_headers}"
# )
