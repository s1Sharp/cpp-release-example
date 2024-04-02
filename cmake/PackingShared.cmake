# Now you can install the targets, including libhiredisd_shared
install(
    TARGETS
        fancy-version
        fancy-cli
        fancy-ser
        hiredis
        redis++
    LIBRARY
        DESTINATION lib
    COMPONENT libraries
)

install(
	TARGETS
		fancy_client
		fancy_server
	RUNTIME
		DESTINATION bin
	PERMISSIONS
		OWNER_EXECUTE OWNER_WRITE OWNER_READ
		GROUP_EXECUTE			  GROUP_READ
		WORLD_EXECUTE
	COMPONENT applications
)

set(PACKAGE_NAME_TO_RELEASE "${PROJECT_NAME}-shared")

set(CPACK_PACKAGE_NAME ${PACKAGE_NAME_TO_RELEASE})


# Настраиваем CPack
set(CPACK_GENERATOR DEB)

set(CPACK_DEBIAN_PACKAGE_INCLUDE_IN_DEBUG FALSE)

set(CPACK_INSTALL_CMAKE_PROJECTS
    "${CMAKE_CURRENT_BINARY_DIR};${CMAKE_PROJECT_NAME};applications;/"
    "${CMAKE_CURRENT_BINARY_DIR};${CMAKE_PROJECT_NAME};libraries;/"
)
set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)
set(CPACK_COMPONENTS_ALL applications libraries)

# which is useful in case of packing only selected components instead of the whole thing
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simple C++ application to check your fancy"
    CACHE STRING "Package description for the package metadata"
)
set(CPACK_PACKAGE_VENDOR "s1Sharp")

set(CPACK_VERBATIM_VARIABLES YES)

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/_packages")

# https://unix.stackexchange.com/a/11552/254512
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")

set(CPACK_SYSTEM_NAME "${OS_SYSTEM_NAME}")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}")

set(CPACK_PACKAGE_CONTACT "maksim.carkov.201300@gmail.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "s1Sharp")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA, "${CMAKE_CURRENT_SOURCE_DIR}/debian/postinst;${CMAKE_CURRENT_SOURCE_DIR}/debian/postinst")
set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION TRUE)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)

# package name for deb. If set, then instead of some-application-0.9.2-Linux.deb
# you'll get some-application_0.9.2_amd64.deb (note the underscores too)
# set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

# that is if you want every group to have its own package,
# although the same will happen if this is not set (so it defaults to ONE_PER_GROUP)
# and CPACK_DEB_COMPONENT_INSTALL is set to YES

# set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)
# without this you won't be able to pack only specified component
# set(CPACK_DEB_COMPONENT_INSTALL YES)

include(CPack)
