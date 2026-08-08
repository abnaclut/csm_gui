// ============================================================================
// demo_layer.hpp - Demo UI layer
// ============================================================================
#ifndef CSM_GUI_DEMO_LAYER_HPP
#define CSM_GUI_DEMO_LAYER_HPP

#include <functional>
#include <gui_shell/layer.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <string>
#include <vector>
#include "logger.hpp"
#include "theme.hpp"

//----------------------------------------------------------------------
/// Demo layer with some UI elements (for tests)
/// OnUpdate() - logic only, OnImGuiRender() - rendering only
class DemoLayer final : public gui_shell::Layer
{
public:
  DemoLayer() : Layer("DemoLayer") {}

  void OnAttach() override
  {
    GS_INFO("DemoLayer attached");
    setupDemoPages();
  }

  void OnDetach() override { GS_INFO("DemoLayer detached"); }

  void OnUpdate(const float dt) override
  {
    m_elapsed += dt;

    // Animations
    m_anim = ImLerp(m_anim, m_anim_dst, 8.0f * dt);
    if (std::abs(m_anim - m_anim_dst) < 0.001f)
    {
      m_anim_dst = 1.0f;
      m_current_tab = m_next_tab;
    }

    m_content_anim = ImLerp(m_content_anim, m_content_anim_dst, 8.0f * dt);
    if (std::abs(m_content_anim - m_content_anim_dst) < 0.001f)
    {
      m_content_anim_dst = 1.0f;
      m_current_subtab = m_next_subtab;
    }
  }

  void OnImGuiRender() override
  {
    ImGui::SetNextWindowSize(ImVec2(670, 478), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

    ImGui::Begin("CSM GUI Demo", nullptr, ImGuiWindowFlags_NoCollapse);

    renderMainLayout();

    ImGui::End();

    if (m_show_demo) { ImGui::ShowDemoWindow(&m_show_demo); }
  }

private:
  // ========================================================================
  // Data structures
  // ========================================================================

  struct Tab
  {
    const char* icon;
    const char* name;
    std::vector<const char*> subtabs;
    std::vector<std::function<void()>> pages;
    int current_subtab = 0;
    int next_subtab = 0;

    Tab(const char* ic, const char* nm, std::vector<const char*> subs = {})
      : icon(ic), name(nm), subtabs(std::move(subs)) {}
  };

  // ========================================================================
  // Demo pages setup
  // ========================================================================

  void setupDemoPages()
  {
    // "General" page
    m_tabs[0].pages.push_back([this]() { renderGeneralPage(); });

    // "Weapons" page
    m_tabs[0].pages.push_back([this]() { renderWeaponsPage(); });

    // "Misc" page
    m_tabs[0].pages.push_back([this]() { renderMiscPage(); });

    // "Visuals" page
    m_tabs[1].pages.push_back([this]() { renderVisualsPage(); });

    // "Settings" page
    m_tabs[2].pages.push_back([this]() { renderSettingsPage(); });
  }

  // ========================================================================
  // Page rendering
  // ========================================================================

  void renderGeneralPage()
  {
    ImGui::BeginGroup();
    {
      beginChild("General", ImVec2(0, 0));
      ImGui::Checkbox("Enabled", &m_bools[0]);
      ImGui::Checkbox("Disabled", &m_bools[1]);

      const char* items[] = { "Value 1", "Value 2", "Value 3", "Value 4" };
      ImGui::Combo("Combobox", &m_ints[0], items, IM_ARRAYSIZE(items));

      ImGui::InputText("Textfield", m_buf, sizeof(m_buf));
      ImGui::SliderInt("Slider", &m_ints[1], 0, 100, "(%d%%)");
      endChild();
    }
    ImGui::EndGroup();

    ImGui::SameLine();

    ImGui::BeginGroup();
    {
      beginChild("Weapons", ImVec2(0, ImGui::GetWindowHeight() / 2 - 10));
      colorPicker("Colorpicker", m_color);
      endChild();

      beginChild("Miscellaneous", ImVec2(0, 0));
      for (int i = 0; i < 8; ++i)
      {
        ImGui::Checkbox(("Checkbox " + std::to_string(i)).c_str(), &m_bools[2 + i]);
      }
      endChild();
    }
    ImGui::EndGroup();
  }

  void renderWeaponsPage()
  {
    beginChild("Weapon Settings", ImVec2(0, 0));
    ImGui::Text("Weapon configuration");
    ImGui::Separator();

    static int weapon_index = 0;
    const char* weapons[] = { "AK-47", "M4A4", "AWP", "Deagle" };
    ImGui::Combo("Weapon", &weapon_index, weapons, IM_ARRAYSIZE(weapons));

    ImGui::Checkbox("Enable weapon", &m_bools[10]);
    ImGui::SliderFloat("Damage", &m_float_values[0], 0.0f, 100.0f, "%.1f");
    ImGui::SliderInt("Magazine", &m_ints[3], 0, 30);

    endChild();
  }

  void renderMiscPage()
  {
    beginChild("Misc Settings", ImVec2(0, 0));
    ImGui::Text("Miscellaneous settings");
    ImGui::Separator();

    ImGui::Checkbox("Show ImGui demo", &m_show_demo);
    ImGui::Checkbox("Enable logging", &m_bools[11]);

    const char* log_levels[] = { "Debug", "Info", "Warning", "Error" };
    ImGui::Combo("Log level", &m_ints[4], log_levels, IM_ARRAYSIZE(log_levels));

    ImGui::InputFloat("Sensitivity", &m_float_values[1], 0.1f, 1.0f);
    ImGui::InputInt("FOV", &m_ints[5]);

    endChild();
  }

  void renderVisualsPage()
  {
    beginChild("Visual Settings", ImVec2(0, 0));

    ImGui::Text("Visuals configuration");
    ImGui::Separator();

    ImGui::Checkbox("Enable visuals", &m_bools[12]);
    ImGui::Checkbox("Player ESP", &m_bools[13]);
    ImGui::Checkbox("Box ESP", &m_bools[14]);
    ImGui::Checkbox("Skeleton", &m_bools[15]);

    ImGui::Separator();
    ImGui::ColorEdit4("Color", m_color2, ImGuiColorEditFlags_NoInputs);
    ImGui::SliderFloat("Opacity", &m_float_values[2], 0.0f, 1.0f);

    endChild();
  }

  void renderSettingsPage()
  {
    beginChild("Settings", ImVec2(0, 0));

    ImGui::Text("Application Settings");
    ImGui::Separator();

    ImGui::Text("Theme");
    if (ImGui::BeginCombo("##theme", m_theme_names[m_theme_index]))
    {
      for (int i = 0; i < IM_ARRAYSIZE(m_theme_names); ++i)
      {
        bool selected = (m_theme_index == i);
        if (ImGui::Selectable(m_theme_names[i], selected))
        {
          m_theme_index = i;
          gui_shell::Theme::Apply(gui_shell::Theme::FromName(m_theme_names[i]));
        }
      }
      ImGui::EndCombo();
    }

    ImGui::Separator();

    if (ImGui::Button("Save Settings", ImVec2(120, 30)))
    {
      GS_INFO("Settings saved!");
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Settings", ImVec2(120, 30)))
    {
      GS_INFO("Settings loaded!");
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset", ImVec2(120, 30)))
    {
      GS_INFO("Settings reset!");
    }

    endChild();
  }

  // ========================================================================
  // UI Helpers (in the style of your project)
  // ========================================================================

  void beginChild(const char* name, ImVec2 size)
  {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetColorU32(ImGuiCol_WindowBg));
    ImGui::BeginChild(("##" + std::string(name)).c_str(), size, ImGuiChildFlags_Border);
    ImGui::TextUnformatted(name);
    ImGui::Separator();
    ImGui::Spacing();
  }

  void endChild()
  {
    ImGui::EndChild();
    ImGui::PopStyleColor();
  }

  bool colorPicker(const char* name, float col[4], bool alpha = true)
  {
    ImGui::TextUnformatted(name);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    return ImGui::ColorEdit4(("##" + std::string(name)).c_str(), col,
      (alpha ? ImGuiColorEditFlags_AlphaBar : ImGuiColorEditFlags_NoAlpha) |
      ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
  }

  // ========================================================================
  // Main layout rendering
  // ========================================================================

  void renderMainLayout()
  {
    ImGui::BeginChild("nav", ImVec2(176, 0), ImGuiChildFlags_Border);

    // Brand / Logo
    ImGui::BeginChild("brand", ImVec2(0, 42), ImGuiChildFlags_None);
    ImGui::TextUnformatted("CSM");
    ImGui::Separator();
    ImGui::EndChild();

    // Tabs
    for (int i = 0; i < static_cast<int>(m_tabs.size()); ++i)
    {
      renderTab(i);
    }

    // Profile
    ImGui::BeginChild("profile", ImVec2(0, 42), ImGuiChildFlags_None);
    ImGui::TextUnformatted("User");
    ImGui::EndChild();

    ImGui::EndChild();

    // Main content
    ImGui::SameLine();
    ImGui::BeginChild("main", ImVec2(0, 0), ImGuiChildFlags_None);

    // Header
    ImGui::BeginChild("header", ImVec2(0, 42), ImGuiChildFlags_None);
    ImGui::TextUnformatted(m_tabs[m_current_tab].name);
    if (!m_tabs[m_current_tab].subtabs.empty())
    {
      ImGui::SameLine();
      ImGui::TextUnformatted(">");
      ImGui::SameLine();
      ImGui::TextUnformatted(m_tabs[m_current_tab].subtabs[m_current_subtab]);
    }
    ImGui::EndChild();

    // Content
    ImGui::BeginChild("content", ImVec2(0, 0), ImGuiChildFlags_Border);
    renderPageContent();
    ImGui::EndChild();

    ImGui::EndChild();
  }

  void renderTab(int index)
  {
    const auto& tab = m_tabs[index];
    bool selected = (m_current_tab == index);

    ImGui::PushID(index);

    if (ImGui::Selectable((" " + std::string(tab.icon) + " " + tab.name).c_str(), selected))
    {
      if (!selected)
      {
        m_next_tab = index;
        m_anim_dst = 0.0f;
      }
    }

    ImGui::PopID();
  }

  void renderPageContent()
  {
    const auto& tab = m_tabs[m_current_tab];

    if (tab.pages.empty())
    {
      ImGui::TextUnformatted("No content");
      return;
    }

    int page_index = (m_current_subtab < static_cast<int>(tab.pages.size()))
      ? m_current_subtab
      : 0;

    tab.pages[page_index]();
  }

  // ========================================================================
  // Data
  // ========================================================================

  // State
  float m_elapsed = 0.0f;
  float m_anim = 1.0f;
  float m_anim_dst = 1.0f;
  float m_content_anim = 1.0f;
  float m_content_anim_dst = 1.0f;

  int m_current_tab = 0;
  int m_next_tab = 0;
  int m_current_subtab = 0;
  int m_next_subtab = 0;

  // UI data
  int m_ints[8] = {0, 50, 0, 30, 0, 90, 0, 0};
  float m_float_values[4] = {75.0f, 1.0f, 0.8f, 0.0f};
  bool m_bools[16] = {false};
  char m_buf[32] = "";
  float m_color[4] = {1.0f, 0.3f, 0.3f, 1.0f};
  float m_color2[4] = {0.2f, 0.6f, 0.8f, 1.0f};

  // Theme
  int m_theme_index = 0;
  const char* m_theme_names[3] = {"Dark", "Light", "ClassicSteam"};

  // UI state
  bool m_show_demo = false;

  // Tabs structure
  std::vector<Tab> m_tabs = {
    {"A", "General", {"General", "Weapons", "Misc"}},
    {"V", "Visuals", {"Visuals"}},
    {"S", "Settings", {"Settings"}}
  };
};
//----------------------------------------------------------------------

//TODO finish me properly

#endif // CSM_GUI_DEMO_LAYER_HPP