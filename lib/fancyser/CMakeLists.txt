set(LIBRARY_NAME fancy-ser)

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
		${COMMON_SOURCE_FILE_LIST}
		${CMAKE_CURRENT_SOURCE_DIR}/src/server.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/session.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/db_service.cpp
)

set(EXTERNN_REDIS_CPLUS_INCLUDE_DIRS
	${HIREDIS_HEADER}
	${redis_plus_plus_SOURCE_DIR}/src
	${redis_plus_plus_SOURCE_DIR}/src/sw/redis++/cxx11
	${redis_plus_plus_SOURCE_DIR}/src/sw/redis++/cxx17
	${redis_plus_plus_SOURCE_DIR}/src/sw/redis++/tls
	${redis_plus_plus_SOURCE_DIR}/src/sw/redis++/no_tls
)

target_include_directories(
		${LIBRARY_NAME}
	PRIVATE
		# where the library itself will look for its internal headers
		${CMAKE_CURRENT_SOURCE_DIR}/src
	PUBLIC
		${EXTERNN_REDIS_CPLUS_INCLUDE_DIRS}
		# where top-level project will look for the library's public headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${COMMON_INCLUDE_FILE_LIST}>
		$<BUILD_INTERFACE:${EXTERNN_REDIS_CPLUS_INCLUDE_DIRS}>
		# where external projects will look for the library's public headers
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Link the spdlog library to your library
target_link_libraries(${LIBRARY_NAME})


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
