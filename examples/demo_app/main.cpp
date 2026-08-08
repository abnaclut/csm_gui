#include <gui_shell/application.hpp>
#include <gui_shell/demo_layer.hpp>
#include <gui_shell/logger.hpp>
#include <imgui.h>
#include <memory>

#include <gui_shell/layer_markdown.hpp>


using namespace gui_shell;

int main()
{
  Settings settings;
  settings.window.title  = "GUI Shell Demo";
  settings.window.width  = 1280;
  settings.window.height = 1280;
  settings.theme_name    = "Dark";

  // Optional: load persisted technical settings if present.
  settings.LoadFromFile("gui_shell_settings.json");

  Application app(settings);
  app.PushLayer(std::make_unique<EmptyLayer>());
  app.Run();

  // Optional: persist settings back out on exit.
  bool saved = app.GetSettings().SaveToFile("gui_shell_settings.json");
  if (!saved) { GS_WARN("Failed to save settings"); }

  return 0;
}
