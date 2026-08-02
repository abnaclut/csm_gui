#include <memory>
#include <imgui.h>
#include <gui_shell/application.hpp>
#include <gui_shell/layer_markdown.hpp>
#include <gui_shell/logger.hpp>


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
  app.PushLayer(std::make_unique<DemoLayer>());
  app.Run();

  // Optional: persist settings back out on exit.
  bool saved = app.GetSettings().SaveToFile("gui_shell_settings.json");
  if (!saved) { GS_WARN("Failed to save settings"); }

  return 0;
}
