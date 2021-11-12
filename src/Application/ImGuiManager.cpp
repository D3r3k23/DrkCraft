#include "ImGuiManager.hpp"

#include "Core/Profiler.hpp"

// #include <imgui/imgui.h>
// #include <imgui_impl_glfw.h>
// #include <imgui_impl_opengl3.h>

namespace DrkCraft
{
    ImGuiManager::ImGuiManager(void)
      : m_blockEvents(true)
    {
        DRK_PROFILE_FUNCTION();
    }

    ImGuiManager::~ImGuiManager(void)
    {
        DRK_PROFILE_FUNCTION();
    }

    void ImGuiManager::begin_frame(void)
    {
        DRK_PROFILE_FUNCTION();
    }

    void ImGuiManager::end_frame(void)
    {
        DRK_PROFILE_FUNCTION();
    }

    void ImGuiManager::on_event(Event& event)
    {
        if (m_blockEvents)
        {
            // ImGuiIo& io = ImGui::GetIO();
            // if (event == EventCategory::Mouse    && io.WantCaptureMouse)    event.set_handled();
            // if (event == EventCategory::Keyboard && io.WantCaptureKeyboard) event.set_handled();
        }
    }

    void ImGuiManager::block_events(bool block)
    {
        m_blockEvents = block;
    }
}
