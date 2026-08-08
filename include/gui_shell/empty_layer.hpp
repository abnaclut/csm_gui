#ifndef CSM_GUI_EMPTY_LAYER_HPP
#define CSM_GUI_EMPTY_LAYER_HPP

#include <gui_shell/layer.hpp>
#include <imgui.h>

//----------------------------------------------------------------------
/// Usage:
///   class MyOverlay final : public EmptyOverlayLayer
///   {
///   public:
///     MyOverlay() : EmptyOverlayLayer("MyOverlay") {}
///
///   protected:
///     void renderContent() override
///     {
///       ImGui::Text("My overlay content");
///     }
///   };
//----------------------------------------------------------------------
class EmptyLayer : public gui_shell::Layer
{
public:
  explicit EmptyLayer(const std::string& name = "EmptyLayer")
    : Layer(name) {}

  void OnAttach() override {}
  void OnDetach() override {}
  void OnUpdate(float dt) override {}

  void OnImGuiRender() override
  {
    // Fullscreen overlay window
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);

    ImGui::Begin(m_name.c_str(), nullptr,
      ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoBringToFrontOnFocus |
      ImGuiWindowFlags_NoBackground |
      ImGuiWindowFlags_NoNav);

    renderContent();

    ImGui::End();

    if (m_show_demo) { ImGui::ShowDemoWindow(&m_show_demo); }
  }

protected:
  virtual void renderContent()
  {
    ImGui::Text("Empty overlay. Override renderContent() to add UI.");
  }

  bool m_show_demo = false;
};

#endif // CSM_GUI_EMPTY_LAYER_HPP