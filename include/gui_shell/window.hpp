#pragma once

#include <functional>

struct GLFWwindow;

namespace gui_shell
{

  struct WindowSettings;

  /// RAII wrapper around a single GLFW window + OpenGL context. Knows nothing
  /// about ImGui or application logic -- purely the OS window/context layer.
  class Window
  {
  public:
    explicit Window(const WindowSettings& settings);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    [[nodiscard]] bool ShouldClose() const;
    void PollEvents() const;
    void SwapBuffers() const;

    void SetVSync(bool enabled);
    [[nodiscard]] bool IsVSync() const { return m_vsync; }

    [[nodiscard]] int Width() const;
    [[nodiscard]] int Height() const;

    ///m_window getter
    [[nodiscard]] GLFWwindow* Native() const { return m_window; }

    using ResizeCallback = std::function<void(int, int)>;
    void SetResizeCallback(ResizeCallback callback) { m_resize_callback = std::move(callback); }

  private:
    GLFWwindow* m_window = nullptr;
    bool m_vsync = true;
    ResizeCallback m_resize_callback;
  };

} // namespace gui_shell
