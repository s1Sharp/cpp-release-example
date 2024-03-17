# https://google.github.io/googletest/quickstart-cmake.html

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(fancy-test "")

# GoogleTest requires at least C++14
target_compile_features(fancy-test
    PUBLIC
        cxx_std_14
)

target_sources(fancy-test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/test_fancy_function.cpp
)

target_link_libraries(fancy-test
    PRIVATE
        GTest::gtest_main
        fancy-version
)

include(GoogleTest)
gtest_discover_tests(fancy-test)
