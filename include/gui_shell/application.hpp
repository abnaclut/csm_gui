#pragma once

#include <memory>
#include <vector>

#include "gui_shell/layer.hpp"
#include "gui_shell/settings.hpp"

namespace gui_shell {

class Window;
class ImGuiRenderer;

/// Top-level orchestrator: owns the Window, the ImGuiRenderer, and the
/// stack of user Layers, and drives the main loop. This is the single
/// entry point an embedding application needs to construct.
///
/// Usage:
///     gui_shell::Settings settings;
///     settings.LoadFromFile("settings.json");
///
///     gui_shell::Application app(settings);
///     app.PushLayer(std::make_unique<MyLayer>());
///     app.Run();
class Application {
public:
    explicit Application(Settings settings = {});
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    /// Takes ownership of the layer, calls OnAttach(), and adds it to the
    /// per-frame update/render stack (in push order).
    void PushLayer(std::unique_ptr<Layer> layer);

    /// Runs the main loop until the window is closed or Close() is called.
    void Run();

    /// Requests the main loop to stop after the current frame.
    void Close();

    Settings& GetSettings() { return m_settings; }
    const Settings& GetSettings() const { return m_settings; }

    Window& GetWindow() { return *m_window; }

private:
    Settings m_settings;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<ImGuiRenderer> m_renderer;
    std::vector<std::unique_ptr<Layer>> m_layers;
    bool m_running = true;
};

} // namespace gui_shell
