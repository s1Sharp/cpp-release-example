cmake_minimum_required(VERSION 3.16...3.24)

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
