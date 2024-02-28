add_executable(fancy-app "")

set_target_properties(
        fancy-app
    PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

target_sources(fancy-app
    PRIVATE
        src/main.cpp
)

# Сборка и установка библиотеки
if(BUILD_SHARED_LIB)
    target_link_libraries(
            fancy-app
        PRIVATE
            fancy-lib
    )
else()
    set_target_properties(fancy-app PROPERTIES LINK_SEARCH_START_STATIC ON)
    set_target_properties(fancy-app PROPERTIES LINK_SEARCH_END_STATIC ON)

    target_link_options(
            fancy-app
        PRIVATE
            -static-libgcc
            -static-libstdc++
            -lc
            -static
    )
    target_link_libraries(
            fancy-app
        PRIVATE
            fancy-lib
    )
endif()


