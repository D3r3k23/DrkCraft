#include "ImGuiManager.hpp"

#include "Core/Assets.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>
#include <imgui_impl/imgui_impl_glfw.h>
#include <imgui_impl/imgui_impl_opengl3.h>

namespace DrkCraft
{
    ImGuiManager::ImGuiManager(GLFWwindow* window)
      : m_blockEvents(true)
    {
        DRK_PROFILE_FUNCTION();

        IMGUI_CHECKVERSION();
        auto context = ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = "data/imgui.ini";
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        setup_style();

        const char* fontPath = DRK_FONT_ASSET_PATH("Kanit-Medium.ttf");
        auto defaultFont = io.Fonts->AddFontFromFileTTF(fontPath, 16.0f);
        DRK_ASSERT_DEBUG(defaultFont, "Could not load default font: {}", fontPath);
        io.FontDefault = defaultFont;
        {
            DRK_PROFILE_SCOPE("imgui_impl init");
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 400");
        }
    }

    ImGuiManager::~ImGuiManager(void)
    {
        DRK_PROFILE_FUNCTION();
        {
            DRK_PROFILE_SCOPE("imgui_impl shutdown");
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
        }
        ImGui::DestroyContext(nullptr);
    }

    void ImGuiManager::begin_frame(void)
    {
        DRK_PROFILE_FUNCTION();
        {
            DRK_PROFILE_SCOPE("imgui_impl newFrame");
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
        }
        ImGui::NewFrame();
    }

    void ImGuiManager::end_frame(void)
    {
        DRK_PROFILE_FUNCTION();

        ImGui::Render();
        {
            DRK_PROFILE_SCOPE("imgui_impl render");
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    }

    void ImGuiManager::on_event(Event& event)
    {
        if (m_blockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            if (event == EventCategory::Mouse    && io.WantCaptureMouse)    event.set_handled();
            if (event == EventCategory::Keyboard && io.WantCaptureKeyboard) event.set_handled();
        }
    }

    void ImGuiManager::block_events(bool block)
    {
        m_blockEvents = block;
    }

    void ImGuiManager::setup_style(void)
    {
        ImGui::StyleColorsDark();
    }
}
