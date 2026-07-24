set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(gui_shell_tests
    test_settings.cpp
    test_theme.cpp
    test_layer.cpp
)

target_link_libraries(gui_shell_tests
    PRIVATE
        gui_shell::gui_shell
        GTest::gtest_main
)
target_compile_features(gui_shell_tests PRIVATE cxx_std_20)

include(GoogleTest)
gtest_discover_tests(gui_shell_tests)
