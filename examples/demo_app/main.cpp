#include <memory>

#include <imgui.h>

#include <gui_shell/application.hpp>
#include <gui_shell/layer.hpp>
#include <gui_shell/logger.hpp>
#include <gui_shell/theme.hpp>

using namespace gui_shell;

namespace {

/// Example layer demonstrating the logic/render split: OnUpdate() only
/// touches state, OnImGuiRender() only draws it.
class DemoLayer : public Layer {
public:
    DemoLayer() : Layer("DemoLayer") {}

    void OnAttach() override { GS_INFO("DemoLayer attached"); }
    void OnDetach() override { GS_INFO("DemoLayer detached"); }

    void OnUpdate(float dt) override {
        m_elapsed += dt;
    }

    void OnImGuiRender() override {
        ImGui::Begin("gui_shell demo");
        ImGui::TextUnformatted("Modular ImGui + GLFW shell");
        ImGui::Separator();
        ImGui::Text("Elapsed time: %.1fs", m_elapsed);
        ImGui::SliderFloat("Value", &m_value, 0.0f, 1.0f);

        ImGui::Spacing();
        ImGui::TextUnformatted("Theme");
        static const char* theme_names[] = {"Dark", "Light", "ClassicSteam"};
        if (ImGui::BeginCombo("##theme", theme_names[m_theme_index])) {
            for (int i = 0; i < IM_ARRAYSIZE(theme_names); ++i) {
                bool selected = (m_theme_index == i);
                if (ImGui::Selectable(theme_names[i], selected)) {
                    m_theme_index = i;
                    Theme::Apply(Theme::FromName(theme_names[i]));
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Checkbox("Show ImGui demo window", &m_show_demo);
        ImGui::End();

        if (m_show_demo) {
            ImGui::ShowDemoWindow(&m_show_demo);
        }
    }

private:
    float m_elapsed = 0.0f;
    float m_value = 0.5f;
    int m_theme_index = 0;
    bool m_show_demo = false;
};

} // namespace

int main() {
    Settings settings;
    settings.window.title = "GUI Shell Demo";
    settings.window.width = 1280;
    settings.window.height = 800;
    settings.theme_name = "Dark";

    // Optional: load persisted technical settings if present.
    settings.LoadFromFile("gui_shell_settings.json");

    Application app(settings);
    app.PushLayer(std::make_unique<DemoLayer>());
    app.Run();

    // Optional: persist settings back out on exit.
    app.GetSettings().SaveToFile("gui_shell_settings.json");

    return 0;
}
