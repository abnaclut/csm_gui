#pragma once

namespace gui_shell
{

  class Window;

  /// Owns the ImGui context and the GLFW/OpenGL3 backends. Responsible only
  /// for frame lifecycle (begin/clear/end) -- no widget/business logic lives
  /// here, that belongs in user-supplied Layers.
  class ImGuiRenderer
  {
  public:
    explicit ImGuiRenderer(Window& window);
    ~ImGuiRenderer();

    ImGuiRenderer(const ImGuiRenderer&) = delete;
    ImGuiRenderer& operator=(const ImGuiRenderer&) = delete;

    static void BeginFrame();

    /// Clears the framebuffer with the given RGBA color. Call after UI has
    /// been built (BeginFrame + widget calls) but before EndFrame.
    void Clear(const float color[4]) const;

    /// Finalizes and submits the ImGui draw data for this frame.
    static void EndFrame();

  private:
    Window& m_window;
  };

} // namespace gui_shell
