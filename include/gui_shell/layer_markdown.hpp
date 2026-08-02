#ifndef LAYER_MARKDOWN_HPP
#define LAYER_MARKDOWN_HPP

#include <gui_shell/layer.hpp>
#include <gui_shell/logger.hpp>
#include <gui_shell/theme.hpp>
#include <imgui.h>

//----------------------------------------------------------------------
/// Example layer demonstrating the logic/render split: OnUpdate() only
/// touches state, OnImGuiRender() only draws it.
class DemoLayer final : public gui_shell::Layer
{
public:
  DemoLayer() : Layer("DemoLayer") {}

  void OnAttach() override { GS_INFO("DemoLayer attached"); }
  void OnDetach() override { GS_INFO("DemoLayer detached"); }

  void OnUpdate(const float dt) override { m_elapsed += dt; }

  void OnImGuiRender() override
  {
    ImGui::Begin("gui_shell demo");
    ImGui::TextUnformatted("Modular ImGui + GLFW shell");
    ImGui::Separator();
    ImGui::Text("Elapsed time: %.1fs", m_elapsed);
    ImGui::SliderFloat("Value", &m_value, 0.0f, 1.0f);

    ImGui::Spacing();
    ImGui::TextUnformatted("Theme");
    static const char* theme_names[] = {"Dark", "Light", "ClassicSteam"};
    if (ImGui::BeginCombo("##theme", theme_names[m_theme_index]))
    {
      for (int i = 0; i < IM_ARRAYSIZE(theme_names); ++i)
      {
        bool selected = (m_theme_index == i);
        if (ImGui::Selectable(theme_names[i], selected))
        {
          m_theme_index = i;
          gui_shell::Theme::Apply(gui_shell::Theme::FromName(theme_names[i]));
        }
      }
      ImGui::EndCombo();
    }

    ImGui::Checkbox("Show ImGui demo window", &m_show_demo);
    ImGui::End();

    if (m_show_demo) { ImGui::ShowDemoWindow(&m_show_demo); }
  }

private:
  float m_elapsed     = 0.0f;
  float m_value       = 0.5f;
  int   m_theme_index = 0;
  bool  m_show_demo   = false;
};
//----------------------------------------------------------------------

#endif // LAYER_MARKDOWN_HPP
