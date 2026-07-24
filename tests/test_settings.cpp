#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

#include "gui_shell/settings.hpp"

using namespace gui_shell;

namespace {

class SettingsTest : public ::testing::Test {
protected:
    std::filesystem::path temp_path = std::filesystem::temp_directory_path() / "gui_shell_test_settings.json";

    void TearDown() override {
        std::error_code ec;
        std::filesystem::remove(temp_path, ec);
    }
};

TEST_F(SettingsTest, SaveAndLoadRoundTrip) {
    Settings original;
    original.window.title = "Test App";
    original.window.width = 1024;
    original.window.height = 600;
    original.window.vsync = false;
    original.window.msaa_samples = 8;
    original.render.target_fps = 60;
    original.render.clear_color[0] = 0.25f;
    original.theme_name = "Light";

    ASSERT_TRUE(original.SaveToFile(temp_path));

    Settings loaded;
    ASSERT_TRUE(loaded.LoadFromFile(temp_path));

    EXPECT_EQ(loaded.window.title, "Test App");
    EXPECT_EQ(loaded.window.width, 1024);
    EXPECT_EQ(loaded.window.height, 600);
    EXPECT_FALSE(loaded.window.vsync);
    EXPECT_EQ(loaded.window.msaa_samples, 8);
    EXPECT_EQ(loaded.render.target_fps, 60);
    EXPECT_FLOAT_EQ(loaded.render.clear_color[0], 0.25f);
    EXPECT_EQ(loaded.theme_name, "Light");
}

TEST_F(SettingsTest, LoadMissingFileReturnsFalseAndKeepsDefaults) {
    Settings s;
    EXPECT_FALSE(s.LoadFromFile("/nonexistent/path/gui_shell_settings.json"));
    EXPECT_EQ(s.theme_name, "Dark");
}

TEST_F(SettingsTest, DefaultsAreSane) {
    Settings s;
    EXPECT_GT(s.window.width, 0);
    EXPECT_GT(s.window.height, 0);
    EXPECT_FALSE(s.window.title.empty());
    EXPECT_EQ(s.theme_name, "Dark");
}

TEST_F(SettingsTest, LoadIgnoresMissingFieldsGracefully) {
    std::ofstream(temp_path) << R"({"window": {"title": "Partial"}})";

    Settings s;
    ASSERT_TRUE(s.LoadFromFile(temp_path));

    // Field present in the file is applied...
    EXPECT_EQ(s.window.title, "Partial");
    // ...fields absent from the file fall back to WindowSettings' own
    // defaults (not whatever the target held before loading).
    EXPECT_EQ(s.window.width, WindowSettings{}.width);
    EXPECT_EQ(s.window.height, WindowSettings{}.height);
}

} // namespace
