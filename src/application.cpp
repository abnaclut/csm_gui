#include <gui_shell/application.hpp>

#include <chrono>
#include <thread>

#include <gui_shell/logger.hpp>
#include <gui_shell/renderer.hpp>
#include <gui_shell/theme.hpp>
#include <gui_shell/window.hpp>

namespace gui_shell
{

  Application::Application(Settings settings) : m_settings(std::move(settings))
  {
    Logger::Init();

    m_window   = std::make_unique<Window>(m_settings.window);
    m_renderer = std::make_unique<ImGuiRenderer>(*m_window);

    Theme::Apply(Theme::FromName(m_settings.theme_name));

    GS_INFO("Application initialized");
  }

  Application::~Application()
  {
    m_layers.clear();
    m_renderer.reset();
    m_window.reset();
  }

  void Application::PushLayer(std::unique_ptr<Layer> layer)
  {
    GS_INFO("Pushing layer: {}", layer->Name());
    layer->OnAttach();
    m_layers.push_back(std::move(layer));
  }

  void Application::Close() { m_running = false; }

  void Application::Run() const
  {
    using clock     = std::chrono::steady_clock;
    auto last_frame = clock::now();

    while (m_running && !m_window->ShouldClose())
    {
      const auto  frame_start = clock::now();
      const float dt          = std::chrono::duration<float>(frame_start - last_frame).count();
      last_frame              = frame_start;

      m_window->PollEvents();

      // --- logic pass -----------------------------------------------
      for (auto& layer : m_layers) { layer->OnUpdate(dt); }

      // --- render pass -------------------------------------------------
      m_renderer->BeginFrame();
      for (auto& layer : m_layers) { layer->OnImGuiRender(); }
      m_renderer->Clear(m_settings.render.clear_color);
      m_renderer->EndFrame();
      m_window->SwapBuffers();

      // Optional soft FPS cap, independent of vsync.
      if (m_settings.render.target_fps > 0)
      {
        const auto frame_duration =
          std::chrono::duration<float>(1.0f / static_cast<float>(m_settings.render.target_fps));
        const auto elapsed = clock::now() - frame_start;
        if (elapsed < frame_duration) { std::this_thread::sleep_for(frame_duration - elapsed); }
      }
    }

    for (auto& layer : m_layers) { layer->OnDetach(); }

    GS_INFO("Application main loop exited");
  }

} // namespace gui_shell
