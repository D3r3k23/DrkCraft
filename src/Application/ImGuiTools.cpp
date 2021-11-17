#include "ImGuiTools.hpp"

#include "Core/AssetManager.hpp"
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
        io.IniFilename = nullptr; // "data/imgui.ini";
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        auto fontPath = font_asset_path("Kanit-Medium.ttf");
        ImFont* font;

        font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 16.0f);
        DRK_ASSERT_DEBUG(font, "Could not load font: {}", fontPath);
        s_fonts[Font::Regular] = font;
        io.FontDefault = font;

        font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 40.0f);
        DRK_ASSERT_DEBUG(font, "Could not load font: {}", fontPath);
        s_fonts[Font::Title] = font;

        setup_style();

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

    void ImGuiManager::enable(void)
    {
        m_enabled = true;
    }

    void ImGuiManager::disable(void)
    {
        m_enabled = false;
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
        void BeginFullscreen(std::string_view name, ImGuiWindowFlags flags)
        {
            const auto viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);

            flags |= ImGuiWindowFlags_NoDecoration    | ImGuiWindowFlags_NoMove
                   | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize;

            ImGui::Begin(name.data(), nullptr, flags);
        }

        void BeginCentered(std::string_view name, const ImVec2& size, ImGuiWindowFlags flags)
        {
            // ImGui::SetNextWindowSize(WINDOW_SIZE);
            // float x = (ImGui::GetWindowContentRegionMax().x - WINDOW_SIZE.x) * 0.5f;
            // float y = (ImGui::GetCursorPos().y + WINDOW_SIZE.y) * 0.5;
            // ImGui::SetNextWindowPos({x, y});

            ImGui::SetNextWindowSize(size);

            const auto viewport = ImGui::GetMainViewport();
            float x = (viewport->Pos.x + (viewport->Size.x - size.x * 0.5f));
            float y = (viewport->Pos.y + (viewport->Size.y - size.y * 0.5f));

            ImGui::SetNextWindowPos({x, y});

            flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
            ImGui::Begin(name.data(), nullptr, flags);
        }

        void TextCentered(std::string_view text)
        {
            float width = ImGui::CalcTextSize(text.data()).x;
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - width) * 0.5f);
            ImGui::Text(text.data());
        }

        bool ButtonCentered(const char* text, const ImVec2& size)
        {
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - size.x) * 0.5f);
            return ImGui::Button(text, size);
        }
    }
}
