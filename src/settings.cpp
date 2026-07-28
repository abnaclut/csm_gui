#include <gui_shell/settings.hpp>

#include <fstream>

#include <nlohmann/json.hpp>

#include <gui_shell/logger.hpp>

using json = nlohmann::json;

namespace gui_shell
{

  // NOTE: these must be findable via ADL from inside nlohmann::adl_serializer,
  // which only looks in namespaces associated with the argument types (i.e.
  // gui_shell itself) -- so they live directly in gui_shell, not in a nested
  // anonymous namespace.

  void to_json(json& j, const WindowSettings& w)
  {
    j = json{
      {"title", w.title},
      {"width", w.width},
      {"height", w.height},
      {"resizable", w.resizable},
      {"start_maximized", w.start_maximized},
      {"vsync", w.vsync},
      {"msaa_samples", w.msaa_samples},
    };
  }

  void from_json(const json& j, WindowSettings& w)
  {
    w.title           = j.value("title", w.title);
    w.width           = j.value("width", w.width);
    w.height          = j.value("height", w.height);
    w.resizable       = j.value("resizable", w.resizable);
    w.start_maximized = j.value("start_maximized", w.start_maximized);
    w.vsync           = j.value("vsync", w.vsync);
    w.msaa_samples    = j.value("msaa_samples", w.msaa_samples);
  }

  void to_json(json& j, const RenderSettings& r)
  {
    j = json{
      {"clear_color", {r.clear_color[0], r.clear_color[1], r.clear_color[2], r.clear_color[3]}},
      {"target_fps", r.target_fps},
    };
  }

  void from_json(const json& j, RenderSettings& r)
  {
    if (j.contains("clear_color"))
    {
      const auto& c = j.at("clear_color");
      for (size_t i = 0; i < 4 && i < c.size(); ++i) { r.clear_color[i] = c.at(i).get<float>(); }
    }
    r.target_fps = j.value("target_fps", r.target_fps);
  }

  bool Settings::LoadFromFile(const std::filesystem::path& path)
  {
    std::ifstream file(path);
    if (!file.is_open())
    {
      GS_WARN("Settings file not found at '{}', keeping current values", path.string());
      return false;
    }

    try
    {
      json j;
      file >> j;
      if (j.contains("window")) { window = j.at("window").get<WindowSettings>(); }
      if (j.contains("render")) { render = j.at("render").get<RenderSettings>(); }
      theme_name = j.value("theme_name", theme_name);
    }
    catch (const std::exception& e)
    {
      GS_ERROR("Failed to parse settings file '{}': {}", path.string(), e.what());
      return false;
    }

    GS_INFO("Loaded settings from '{}'", path.string());
    return true;
  }

  bool Settings::SaveToFile(const std::filesystem::path& path) const
  {
    json j;
    j["window"]     = window;
    j["render"]     = render;
    j["theme_name"] = theme_name;

    std::ofstream file(path);
    if (!file.is_open())
    {
      GS_ERROR("Failed to open '{}' for writing", path.string());
      return false;
    }

    file << j.dump(4);
    GS_INFO("Saved settings to '{}'", path.string());
    return true;
  }

} // namespace gui_shell
