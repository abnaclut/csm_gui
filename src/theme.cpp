#include <gui_shell/theme.hpp>

#include <gui_shell/logger.hpp>

namespace gui_shell
{

  void Theme::ApplySizing(const ThemeSpec& spec)
  {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding    = spec.rounding;
    style.ChildRounding     = spec.rounding * 0.6f;
    style.FrameRounding     = spec.rounding * 0.6f;
    style.PopupRounding     = spec.rounding * 0.6f;
    style.ScrollbarRounding = spec.rounding;
    style.GrabRounding      = spec.rounding * 0.6f;
    style.TabRounding       = spec.rounding * 0.6f;

    style.WindowBorderSize = spec.window_border_size;
    style.FramePadding     = ImVec2(spec.frame_padding_x, spec.frame_padding_y);
    style.ItemSpacing      = ImVec2(8.0f, 6.0f);
    style.WindowPadding    = ImVec2(12.0f, 12.0f);
  }

  void Theme::ApplyDark(const ThemeSpec& spec)
  {
    ImGui::StyleColorsDark();
    ImGuiStyle&   style  = ImGui::GetStyle();
    ImVec4*       colors = style.Colors;
    const ImVec4& accent = spec.accent_color;

    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.105f, 0.11f, 1.00f);
    colors[ImGuiCol_ChildBg]  = ImVec4(0.10f, 0.105f, 0.11f, 1.00f);
    colors[ImGuiCol_PopupBg]  = ImVec4(0.08f, 0.08f, 0.09f, 0.98f);
    colors[ImGuiCol_Border]   = ImVec4(0.20f, 0.21f, 0.22f, 0.60f);

    colors[ImGuiCol_FrameBg]        = ImVec4(0.16f, 0.17f, 0.18f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.24f, 0.26f, 0.28f, 1.00f);

    colors[ImGuiCol_TitleBg]          = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgActive]    = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.09f, 0.75f);

    colors[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);

    colors[ImGuiCol_Header]        = ImVec4(accent.x, accent.y, accent.z, 0.45f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(accent.x, accent.y, accent.z, 0.70f);
    colors[ImGuiCol_HeaderActive]  = ImVec4(accent.x, accent.y, accent.z, 0.90f);

    colors[ImGuiCol_Button]        = ImVec4(accent.x, accent.y, accent.z, 0.55f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(accent.x, accent.y, accent.z, 0.78f);
    colors[ImGuiCol_ButtonActive]  = accent;

    colors[ImGuiCol_Tab]                = ImVec4(0.14f, 0.15f, 0.16f, 1.00f);
    colors[ImGuiCol_TabHovered]         = ImVec4(accent.x, accent.y, accent.z, 0.78f);
    colors[ImGuiCol_TabActive]          = ImVec4(accent.x, accent.y, accent.z, 0.55f);
    colors[ImGuiCol_TabUnfocused]       = ImVec4(0.11f, 0.12f, 0.13f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.16f, 0.17f, 0.18f, 1.00f);

    colors[ImGuiCol_CheckMark]        = accent;
    colors[ImGuiCol_SliderGrab]       = accent;
    colors[ImGuiCol_SliderGrabActive] = accent;

    colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.24f, 0.25f, 0.27f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.31f, 0.33f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]  = accent;

    colors[ImGuiCol_PlotLines]     = accent;
    colors[ImGuiCol_PlotHistogram] = accent;

    colors[ImGuiCol_Separator]         = ImVec4(0.20f, 0.21f, 0.22f, 1.00f);
    colors[ImGuiCol_ResizeGrip]        = ImVec4(accent.x, accent.y, accent.z, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(accent.x, accent.y, accent.z, 0.55f);
    colors[ImGuiCol_ResizeGripActive]  = ImVec4(accent.x, accent.y, accent.z, 0.80f);

    ApplySizing(spec);
  }

  void Theme::ApplyLight(const ThemeSpec& spec)
  {
    ImGui::StyleColorsLight();
    ImGuiStyle& style                   = ImGui::GetStyle();
    style.Colors[ImGuiCol_Header]       = spec.accent_color;
    style.Colors[ImGuiCol_ButtonActive] = spec.accent_color;
    style.Colors[ImGuiCol_CheckMark]    = spec.accent_color;

    ApplySizing(spec);
  }

  void Theme::ApplyClassicSteam(const ThemeSpec& spec)
  {
    // Cool blue-gray "control panel" look layered on top of the Dark base.
    ApplyDark(spec);
    ImGuiStyle& style                    = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg]      = ImVec4(0.086f, 0.106f, 0.129f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.106f, 0.137f, 0.169f, 1.00f);
    style.Colors[ImGuiCol_FrameBg]       = ImVec4(0.14f, 0.17f, 0.20f, 1.00f);
  }

  void Theme::Apply(const ThemeSpec& spec)
  {
    switch (spec.preset)
    {
    case ThemePreset::Dark:
      ApplyDark(spec);
      break;
    case ThemePreset::Light:
      ApplyLight(spec);
      break;
    case ThemePreset::ClassicSteam:
      ApplyClassicSteam(spec);
      break;
    case ThemePreset::Custom:
      ApplyDark(spec);
      break;
    }
    GS_INFO("Applied theme: {}", ToName(spec.preset));
  }

  void Theme::ApplyPreset(ThemePreset preset)
  {
    ThemeSpec spec;
    spec.preset = preset;
    Apply(spec);
  }

  std::string Theme::ToName(ThemePreset preset)
  {
    switch (preset)
    {
    case ThemePreset::Dark:
      return "Dark";
    case ThemePreset::Light:
      return "Light";
    case ThemePreset::ClassicSteam:
      return "ClassicSteam";
    case ThemePreset::Custom:
      return "Custom";
    }
    return "Dark";
  }

  ThemeSpec Theme::FromName(const std::string& name)
  {
    ThemeSpec spec;
    if      (name == "Light")        { spec.preset = ThemePreset::Light; }
    else if (name == "ClassicSteam") { spec.preset = ThemePreset::ClassicSteam; }
    else if (name == "Custom")       { spec.preset = ThemePreset::Custom; }
    else                             { spec.preset = ThemePreset::Dark; }
    return spec;
  }

} // namespace gui_shell
