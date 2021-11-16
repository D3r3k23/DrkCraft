#include "ImGuiTools.hpp"

#include "Core/Assets.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <imgui_impl/imgui_impl_glfw.h>
#include <imgui_impl/imgui_impl_opengl3.h>

namespace DrkCraft
{
    std::unordered_map<Font, ImFont*> ImGuiManager::s_fonts;

    ImGuiManager::ImGuiManager(GLFWwindow* window, bool enable)
      : m_enabled(enable),
        m_blockEvents(true)
    {
        DRK_PROFILE_FUNCTION();

        IMGUI_CHECKVERSION();
        auto context = ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = "data/imgui.ini";
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        setup_style();

        const char* fontPath = DRK_FONT_ASSET_PATH("Kanit-Medium.ttf");
        ImFont* font = nullptr;

        font = io.Fonts->AddFontFromFileTTF(fontPath, 16.0f);
        DRK_ASSERT_DEBUG(font, "Could not load font: {}", fontPath);
        s_fonts[Font::Regular] = font;
        io.FontDefault = font;

        font = io.Fonts->AddFontFromFileTTF(fontPath, 40.0f);
        DRK_ASSERT_DEBUG(font, "Could not load font: {}", fontPath);
        s_fonts[Font::Title] = font;

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

        if (m_enabled)
        {
            {
                DRK_PROFILE_SCOPE("imgui_impl newFrame");
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
            }
            ImGui::NewFrame();
        }
    }

    void ImGuiManager::end_frame(void)
    {
        DRK_PROFILE_FUNCTION();

        static bool showDemoWindow = true;
        if (showDemoWindow)
            ImGui::ShowDemoWindow(&showDemoWindow);

        if (m_enabled)
        {
            ImGui::Render();
            {
                DRK_PROFILE_SCOPE("imgui_impl render");
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }
        }
    }

    void ImGuiManager::on_event(Event& event)
    {
        if (m_enabled && m_blockEvents)
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

    ImFont* ImGuiManager::get_font(Font font)
    {
        DRK_ASSERT_DEBUG(s_fonts.contains(font), "Unknown font");
        return s_fonts[font];
    }

    namespace ImGuiTools
    {
        void TextCentered(std::string_view text)
        {
            float textHeight = ImGui::GetFontSize();
            float textWidth = textHeight * text.size();
            float x = (ImGui::GetContentRegionMax().x - textWidth) * 0.5f;
            float y = (ImGui::GetCursorPos().y + textHeight) * 0.5f;
            ImGui::SetCursorPos({x, y}); //  Or SetCursorPosX
            ImGui::Text(text.data());
        }

        bool ButtonCentered(const char* text, ImVec2 size)
        {
            ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x - size.x) * 0.5f);
            return ImGui::Button(text, size);
        }
    }
}
