#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

namespace gui_shell {

/// Technical, window-manager-level settings. Deliberately free of any
/// rendering/theme concerns so it can be persisted/edited independently.
struct WindowSettings {
    std::string title = "GUI Shell";
    int width = 1280;
    int height = 800;
    bool resizable = true;
    bool start_maximized = false;
    bool vsync = true;
    int msaa_samples = 4;
};

/// Low level render loop settings.
struct RenderSettings {
    float clear_color[4] = {0.10f, 0.105f, 0.11f, 1.0f};
    /// 0 = uncapped (driven purely by vsync); otherwise a soft FPS cap.
    int target_fps = 0;
};

/// Aggregate, serializable settings object for the whole shell. This is the
/// single object an embedding application needs to own, persist, and pass
/// into gui_shell::Application.
struct Settings {
    WindowSettings window;
    RenderSettings render;
    std::string theme_name = "Dark";

    /// Loads settings from a JSON file. Returns false (and leaves *this
    /// untouched) if the file is missing or malformed.
    bool LoadFromFile(const std::filesystem::path& path);

    /// Writes settings to a JSON file. Returns false on I/O failure.
    bool SaveToFile(const std::filesystem::path& path) const;
};

} // namespace gui_shell
