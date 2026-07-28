#include <gui_shell/renderer.hpp>

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <gui_shell/logger.hpp>
#include <gui_shell/window.hpp>

namespace gui_shell
{

  ImGuiRenderer::ImGuiRenderer(Window& window) : m_window(window)
  {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window.Native(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    GS_INFO("ImGuiRenderer initialized (GLFW + OpenGL3 backend)");
  }

  ImGuiRenderer::~ImGuiRenderer()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void ImGuiRenderer::BeginFrame()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void ImGuiRenderer::Clear(const float color[4]) const
  {
    glViewport(0, 0, m_window.Width(), m_window.Height());
    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void ImGuiRenderer::EndFrame()
  {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

} // namespace gui_shell
