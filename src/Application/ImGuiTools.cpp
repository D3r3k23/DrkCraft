#include "ImGuiTools.hpp"

#include "Core/BuildSettings.hpp"
#include "System/AssetManager.hpp"
#include "Core/Profiler.hpp"

#include <imgui_impl/imgui_impl.h>

namespace DrkCraft
{
    std::unordered_map<ImGuiFont, ImFont*> ImGuiManager::s_fonts;

    ImGuiManager::ImGuiManager(Window& window, bool enable)
      : m_enabled(enable),
        m_blockEvents(true),
        m_showDemoWindow(false)
    {
        DRK_PROFILE_FUNCTION();

        IMGUI_CHECKVERSION();
        m_context = ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr; // "data/imgui.ini";

        auto fontPath = font_asset_path("Kanit-Medium.ttf");
        ImFont* font;

        font = io.Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 16.0f);
        DRK_ASSERT_DEBUG(font, "Could not load font: {}", fontPath.generic_string());
        s_fonts[ImGuiFont::Regular] = font;
        io.FontDefault = font;

        font = io.Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 40.0f);
        DRK_ASSERT_DEBUG(font, "Could not load font: {}", fontPath.generic_string());
        s_fonts[ImGuiFont::Button] = font;

        font = io.Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 64.0f);
        DRK_ASSERT_DEBUG(font, "Could not load font: {}", fontPath.generic_string());
        s_fonts[ImGuiFont::Title] = font;

        setup_style();
        init_impl(window.get_raw_window());
    }

    ImGuiManager::~ImGuiManager(void)
    {
        shutdown_impl();
        ImGui::DestroyContext(m_context);
    }

    void ImGuiManager::init_impl(GLFWwindow* window)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Initializing ImGui");

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 400");
    }

    void ImGuiManager::shutdown_impl(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Shutting down ImGui");

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
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

        if (m_enabled)
        {
        #if defined(DRK_CONFIG_DEBUG)
            if (m_showDemoWindow)
                ImGui::ShowDemoWindow(&m_showDemoWindow);
        #endif
            ImGui::Render();
            {
                DRK_PROFILE_SCOPE("imgui_impl render");
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }
        }
    }

    void ImGuiManager::on_event(Event& event)
    {
        if (m_enabled)
        {
            if (m_blockEvents)
            {
                if (should_capture_event(event))
                    event.set_handled();
            }
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

    void ImGuiManager::enable_demo_window(void)
    {
        m_showDemoWindow = true;
    }

    void ImGuiManager::disable_demo_window(void)
    {
        m_showDemoWindow = false;
    }

    void ImGuiManager::toggle_demo_window(void)
    {
        m_showDemoWindow = !m_showDemoWindow;
    }

    bool ImGuiManager::demo_window_enabled(void) const
    {
        return m_showDemoWindow;
    }

    void ImGuiManager::setup_style(void)
    {
        ImGui::StyleColorsDark();
    }

    bool ImGuiManager::should_capture_event(const Event& event) const
    {
        ImGuiIO& io = ImGui::GetIO();
        return (event == EventCategory::Mouse    && io.WantCaptureMouse)
            || (event == EventCategory::Keyboard && io.WantCaptureKeyboard);
    }

    ImFont* ImGuiManager::get_font(ImGuiFont font)
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

            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::Begin(name.data(), nullptr, flags);
            ImGui::PopStyleVar();
        }

        void BeginCentered(std::string_view name, const ImVec2& size, ImGuiWindowFlags flags)
        {
            const auto viewport = ImGui::GetMainViewport();
            float x = (viewport->Pos.x + (viewport->Size.x - size.x) * 0.5f);
            float y = (viewport->Pos.y + (viewport->Size.y - size.y) * 0.5f);

            ImGui::SetNextWindowSize(size);
            ImGui::SetNextWindowPos({x, y});

            flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
            ImGui::Begin(name.data(), nullptr, flags);
        }

        void TextCentered(std::string_view text)
        {
            float width = ImGui::CalcTextSize(text.data()).x;
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - width) * 0.5f);
            ImGui::TextUnformatted(text.data());
        }

        bool ButtonCentered(const char* text, const ImVec2& size)
        {
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - size.x) * 0.5f);
            return ImGui::Button(text, size);
        }
    }
}
