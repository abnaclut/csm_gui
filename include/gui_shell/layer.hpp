#pragma once

#include <string>
#include <utility>

namespace gui_shell
{

  /// Extension point for embedding applications. Each Layer owns a slice of
  /// application logic and/or UI. The Application drives layers through a
  /// well-defined lifecycle, deliberately keeping *logic* (OnUpdate) and
  /// *rendering* (OnImGuiRender) as separate calls so implementers aren't
  /// tempted to mix actual features and drawing in one function.
  class Layer
  {
  public:
    explicit Layer(std::string name = "Layer") : m_name(std::move(name)) {}
    virtual ~Layer() = default;

    Layer(const Layer&) = delete;
    Layer& operator=(const Layer&) = delete;

    /// Called once when the layer is pushed onto the application.
    virtual void OnAttach() {}

    /// Called once when the layer is removed / the application shuts down.
    virtual void OnDetach() {}

    /// Per-frame logic/simulation update. `dt` is seconds since last frame.
    /// Must not issue any ImGui widget calls.
    virtual void OnUpdate(const float dt) { (void)dt; }

    /// Per-frame ImGui widget construction. Called between
    /// ImGuiRenderer::BeginFrame() and EndFrame(); safe to issue any
    /// ImGui:: calls here.
    virtual void OnImGuiRender() {}

    ///name getter
    [[nodiscard]] const std::string& Name() const { return m_name; }

  protected:
    std::string m_name;
  };

} // namespace gui_shell
