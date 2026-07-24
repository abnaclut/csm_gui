#pragma once

#include <string>

#include <imgui.h>

namespace gui_shell {

enum class ThemePreset {
    Dark,
    Light,
    ClassicSteam,
    Custom
};

/// Fully describes an ImGui visual theme: which base preset to start from,
/// plus tunable sizing/color knobs layered on top of it.
struct ThemeSpec {
    ThemePreset preset = ThemePreset::Dark;

    float rounding = 6.0f;
    float window_border_size = 1.0f;
    float frame_padding_x = 8.0f;
    float frame_padding_y = 6.0f;

    ImVec4 accent_color = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
};

/// Pure presentation layer: applies colors/sizing to the *currently active*
/// ImGui context. Has no knowledge of windowing, rendering, or settings
/// persistence, so it can be reused, tested, or swapped independently.
class Theme {
public:
    static void Apply(const ThemeSpec& spec);
    static void ApplyPreset(ThemePreset preset);

    static ThemeSpec FromName(const std::string& name);
    static std::string ToName(ThemePreset preset);

private:
    static void ApplyDark(const ThemeSpec& spec);
    static void ApplyLight(const ThemeSpec& spec);
    static void ApplyClassicSteam(const ThemeSpec& spec);
    static void ApplySizing(const ThemeSpec& spec);
};

} // namespace gui_shell
