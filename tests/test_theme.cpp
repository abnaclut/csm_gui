#include <gtest/gtest.h>

#include "gui_shell/theme.hpp"

using namespace gui_shell;

namespace {

TEST(ThemeTest, FromNameMapsKnownNames) {
    EXPECT_EQ(Theme::FromName("Dark").preset, ThemePreset::Dark);
    EXPECT_EQ(Theme::FromName("Light").preset, ThemePreset::Light);
    EXPECT_EQ(Theme::FromName("ClassicSteam").preset, ThemePreset::ClassicSteam);
    EXPECT_EQ(Theme::FromName("Custom").preset, ThemePreset::Custom);
}

TEST(ThemeTest, FromNameFallsBackToDarkForUnknownInput) {
    EXPECT_EQ(Theme::FromName("NotARealTheme").preset, ThemePreset::Dark);
    EXPECT_EQ(Theme::FromName("").preset, ThemePreset::Dark);
}

TEST(ThemeTest, ToNameRoundTripsThroughFromName) {
    for (auto preset : {ThemePreset::Dark, ThemePreset::Light, ThemePreset::ClassicSteam, ThemePreset::Custom}) {
        const std::string name = Theme::ToName(preset);
        EXPECT_EQ(Theme::FromName(name).preset, preset);
    }
}

TEST(ThemeTest, DefaultSpecHasSaneValues) {
    ThemeSpec spec;
    EXPECT_GE(spec.rounding, 0.0f);
    EXPECT_GE(spec.window_border_size, 0.0f);
}

} // namespace
