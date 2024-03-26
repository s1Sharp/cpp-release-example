cmake_minimum_required(VERSION 3.16...3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -O2")

set(MAJOR_VERSION "1" CACHE INTERNAL "Major version")
set(MINOR_VERSION "1" CACHE INTERNAL "Minor version")
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")

set(PROJECT_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
set(OS_SYSTEM_NAME "linux-dev" CACHE INTERNAL "name of the system")

# Опция для сборки shared библиотеки
option(BUILD_SHARED_LIB "Build shared library" OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(
	fancy-app-example
		VERSION ${PROJECT_VERSION}
		DESCRIPTION "CMake template project containing a library, an app using it, and tests"
		HOMEPAGE_URL "https://github.com/s1Sharp/cpp-release-example"
		LANGUAGES CXX
)

if(BUILD_SHARED_LIB)
	set(Boost_USE_STATIC_LIBS ON)
endif()

find_package(Boost REQUIRED system thread)
find_package(spdlog REQUIRED)

include(FetchContent)

# Загрузка и сборка библиотеки CLI
FetchContent_Declare(
    cli
    GIT_REPOSITORY https://github.com/daniele77/cli.git
    GIT_TAG 	   v2.1.0
	SOURCE_DIR     cli
)
FetchContent_MakeAvailable(cli)

# Download Hiredis, upon which Redis Plus Plus depends.
FetchContent_Declare(
  hiredis
  GIT_REPOSITORY https://github.com/redis/hiredis
  GIT_TAG        v1.1.0
  SOURCE_DIR     hiredis
)

FetchContent_MakeAvailable(hiredis)

# Download the Redis binding.
FetchContent_Declare(
  redis_plus_plus
  GIT_REPOSITORY https://github.com/sewenew/redis-plus-plus
  GIT_TAG        1.3.8
  SOURCE_DIR     redis-plus-plus
)

set(REDIS_PLUS_PLUS_CXX_STANDARD 17)
set(REDIS_PLUS_PLUS_BUILD_SHARED OFF)
set(REDIS_PLUS_PLUS_BUILD_TEST OFF)
# Pre-fill the include and library paths so that Redis++ does not search for them.
set(CMAKE_INCLUDE_PATH "${CMAKE_INCLUDE_PATH};${hiredis_SOURCE_DIR}")
set(HIREDIS_HEADER "${hiredis_SOURCE_DIR}/..")
set(HIREDIS_LIB "${hiredis_BINARY_DIR}/libhiredisd.a")

# # ... include your own source files here.

# # Add the Redis++ include directory.
# target_include_directories(myTarget PRIVATE "${redis_plus_plus_SOURCE_DIR}/src")

# # Link with Redis++ and Hiredis.
# target_link_libraries(myTarget PRIVATE redis++::redis++_static)
# target_link_libraries(myTarget PRIVATE hiredis::hiredis_static)

FetchContent_MakeAvailable(redis_plus_plus)


add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(test EXCLUDE_FROM_ALL)


#### INSTALL PART ####
install(
	TARGETS
		fancy-gui
		server
		client
	RUNTIME
		DESTINATION bin
)

# Устанавливаем папку для установки .so файлов
if(BUILD_SHARED_LIB)
	install(
		TARGETS
			fancy-version
			fancy-cli
			fancy-ser
		LIBRARY
			DESTINATION lib
	)
	set(PACKAGE_NAME_TO_RELEASE "${PROJECT_NAME}-shared")
else()
	set(PACKAGE_NAME_TO_RELEASE "${PROJECT_NAME}-static")
endif()

install(
	TARGETS
		fancy-app
	RUNTIME
		DESTINATION bin
	PERMISSIONS
		OWNER_EXECUTE OWNER_WRITE OWNER_READ
		GROUP_EXECUTE			 GROUP_READ
		WORLD_EXECUTE
)

set(CPACK_PACKAGE_NAME ${PACKAGE_NAME_TO_RELEASE})

# where to find our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Packing)
