#include <gui_shell/window.hpp>

#include <stdexcept>

#include <GLFW/glfw3.h>

#include <gui_shell/logger.hpp>
#include <gui_shell/settings.hpp>

namespace gui_shell
{

  namespace
  {
    void GlfwErrorCallback(int error, const char* description) { GS_ERROR("GLFW error ({}): {}", error, description); }
  } // namespace

  Window::Window(const WindowSettings& settings) : m_vsync(settings.vsync)
  {
    glfwSetErrorCallback(GlfwErrorCallback);

    if (!glfwInit()) { throw std::runtime_error("gui_shell::Window: glfwInit() failed"); }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, settings.msaa_samples);
    glfwWindowHint(GLFW_MAXIMIZED, settings.start_maximized ? GLFW_TRUE : GLFW_FALSE);

    m_window = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
      glfwTerminate();
      throw std::runtime_error("gui_shell::Window: glfwCreateWindow() failed");
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwMakeContextCurrent(m_window);
    SetVSync(settings.vsync);

    glfwSetFramebufferSizeCallback(m_window,
                                   [](GLFWwindow* handle, int width, int height)
                                   {
                                     auto* self = static_cast<Window*>(glfwGetWindowUserPointer(handle));
                                     if (self && self->m_resize_callback)
                                     {
                                       self->m_resize_callback(width, height);
                                     }
                                   });

    GS_INFO("Window created: {}x{} \"{}\" (vsync={})", settings.width, settings.height, settings.title, settings.vsync);
  }

  Window::~Window()
  {
    if (m_window) { glfwDestroyWindow(m_window); }
    glfwTerminate();
  }

  bool Window::ShouldClose() const { return glfwWindowShouldClose(m_window) != 0; }

  void Window::PollEvents() const { glfwPollEvents(); }

  void Window::SwapBuffers() const { glfwSwapBuffers(m_window); }

  void Window::SetVSync(bool enabled)
  {
    m_vsync = enabled;
    glfwSwapInterval(enabled ? 1 : 0);
  }

  int Window::Width() const
  {
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    return width;
  }

  int Window::Height() const
  {
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    return height;
  }

} // namespace gui_shell
