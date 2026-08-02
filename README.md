# gui_shell

A small, modular C++20 GUI shell built on [Dear ImGui](https://github.com/ocornut/imgui)
and [GLFW](https://www.glfw.org/), meant to be dropped into other projects as
the windowing/UI layer. It handles window creation, the ImGui/OpenGL3 render
loop, theming, and technical settings persistence — you just supply `Layer`s
with your own logic and widgets.

## Features

- **CMake + Ninja** build, dependencies fetched automatically via `FetchContent`
  (GLFW, Dear ImGui, spdlog, nlohmann/json, GoogleTest)
- **C++20**, warnings-as-you'd-expect (`-Wall -Wextra -Wpedantic` / `/W4`).
- **spdlog**-backed logging (`GS_INFO`, `GS_WARN`, ... macros).
- **GoogleTest** unit tests for the parts that don't need a live GL context
  (settings, theme, layer lifecycle).
- Clean module separation:

  | Module              | Responsibility                               |
  |----------------------|-----------------------------------------------|
  | `gui_shell::Window`      | GLFW window + OpenGL context               |
  | `gui_shell::ImGuiRenderer` | ImGui context + GLFW/OpenGL3 backend, frame lifecycle |
  | `gui_shell::Theme`       | Pure ImGui style/color presets (Dark/Light/ClassicSteam/Custom) |
  | `gui_shell::Settings`    | Technical settings (window size, vsync, MSAA, FPS cap) + JSON I/O |
  | `gui_shell::Layer`       | Your extension point — logic (`OnUpdate`) and render (`OnImGuiRender`) kept separate |
  | `gui_shell::Application` | Orchestrates the above and runs the main loop |

## Directory layout

```
gui_shell/
├── CMakeLists.txt
├── cmake/                    # dependency fetching + package config
├── include/gui_shell/        # public headers
├── src/                      # implementation
├── tests/                    # GoogleTest unit tests
└── examples/demo_app/        # minimal standalone usage example
```

## Building standalone

Requires CMake ≥ 3.21, Ninja, a C++20 compiler, and OpenGL dev headers
(on Linux: `libgl1-mesa-dev` + `xorg-dev` for GLFW's X11 backend).

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure   # run unit tests
./build/examples/demo_app/gui_shell_demo     # run the demo
```

Useful options (pass as `-DOPTION=ON/OFF`):

| Option                     | Default | Purpose                          |
|-----------------------------|---------|-----------------------------------|
| `GUI_SHELL_BUILD_TESTS`     | `ON`    | Build the GoogleTest suite        |
| `GUI_SHELL_BUILD_EXAMPLES`  | `ON`    | Build `examples/demo_app`         |
| `GUI_SHELL_INSTALL`         | `ON`    | Generate `install()`/export rules |

## Integrating into another project

**A — `add_subdirectory` (simplest, e.g. as a git submodule):**

```cmake
add_subdirectory(third_party/gui_shell)
target_link_libraries(my_app PRIVATE gui_shell::gui_shell)
```

**B — `FetchContent`:**

```cmake
include(FetchContent)
FetchContent_Declare(
    gui_shell
    GIT_REPOSITORY <your-fork-url>
    GIT_TAG        main
)
set(GUI_SHELL_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GUI_SHELL_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gui_shell)

target_link_libraries(my_app PRIVATE gui_shell::gui_shell)
```

Options A and B are recommended: since GLFW/ImGui/spdlog are vendored via
`FetchContent`, `add_subdirectory`/`FetchContent` wires up the
`gui_shell::gui_shell` target and all its dependencies correctly in one
step. `cmake --install` is also available for ad-hoc packaging (copies
headers + the static lib), but does not attempt to re-export the vendored
third-party targets as a relocatable `find_package()` config.

## Minimal usage

```cpp
#include <gui_shell/application.hpp>
#include <gui_shell/layer.hpp>
#include <imgui.h>

class MyLayer : public gui_shell::Layer {
public:
    MyLayer() : Layer("MyLayer") {}

    void OnUpdate(float dt) override {
        // logic only — no ImGui calls here
    }

    void OnImGuiRender() override {
        ImGui::Begin("Hello");
        ImGui::Text("Hello from my project!");
        ImGui::End();
    }
};

int main() {
    gui_shell::Settings settings;
    settings.window.title = "My App";
    settings.theme_name = "ClassicSteam";

    gui_shell::Application app(settings);
    app.PushLayer(std::make_unique<MyLayer>());
    app.Run();
}
```

## Themes

Built-in presets: `Dark`, `Light`, `ClassicSteam`, `Custom`. Switch at
runtime with `gui_shell::Theme::Apply(gui_shell::Theme::FromName("Light"))`,
or tweak a `ThemeSpec` (rounding, accent color, padding) and call
`Theme::Apply(spec)` directly for a fully custom look.

## Settings persistence

`gui_shell::Settings` round-trips to JSON via `LoadFromFile` / `SaveToFile`,
so an embedding app can load a `settings.json` at startup and save it back
on exit — see `examples/demo_app/main.cpp`.
