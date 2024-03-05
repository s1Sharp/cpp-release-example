set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)


set( UIS ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui )
qt5_wrap_ui( UI_HEADERS ${UIS} )

add_executable(fancy-gui
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${UI_HEADERS}
)
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set_target_properties(
        fancy-gui
    PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(
        fancy-gui
    PRIVATE
        fancy-lib
        Qt5::Widgets
)
