#include <gtest/gtest.h>

#include "gui_shell/layer.hpp"

using namespace gui_shell;

namespace {

class CountingLayer : public Layer {
public:
    CountingLayer() : Layer("Counting") {}

    int attach_count = 0;
    int detach_count = 0;
    int update_count = 0;
    int render_count = 0;
    float last_dt = 0.0f;

    void OnAttach() override { ++attach_count; }
    void OnDetach() override { ++detach_count; }
    void OnUpdate(float dt) override {
        ++update_count;
        last_dt = dt;
    }
    void OnImGuiRender() override { ++render_count; }
};

TEST(LayerTest, NameIsStoredAndRetrievable) {
    CountingLayer layer;
    EXPECT_EQ(layer.Name(), "Counting");
}

TEST(LayerTest, DefaultNameIsUsedWhenNoneProvided) {
    class UnnamedLayer : public Layer {};
    UnnamedLayer layer;
    EXPECT_EQ(layer.Name(), "Layer");
}

TEST(LayerTest, LifecycleCallbacksFireExactlyOnce) {
    CountingLayer layer;

    layer.OnAttach();
    layer.OnUpdate(0.016f);
    layer.OnImGuiRender();
    layer.OnDetach();

    EXPECT_EQ(layer.attach_count, 1);
    EXPECT_EQ(layer.update_count, 1);
    EXPECT_EQ(layer.render_count, 1);
    EXPECT_EQ(layer.detach_count, 1);
    EXPECT_FLOAT_EQ(layer.last_dt, 0.016f);
}

TEST(LayerTest, BaseClassDefaultsAreNoOpsAndDontThrow) {
    Layer layer("Base");
    EXPECT_NO_THROW(layer.OnAttach());
    EXPECT_NO_THROW(layer.OnUpdate(0.033f));
    EXPECT_NO_THROW(layer.OnImGuiRender());
    EXPECT_NO_THROW(layer.OnDetach());
}

} // namespace
