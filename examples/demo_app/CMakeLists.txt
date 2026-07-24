add_executable(gui_shell_demo main.cpp)

target_link_libraries(gui_shell_demo PRIVATE gui_shell::gui_shell)
target_compile_features(gui_shell_demo PRIVATE cxx_std_20)
