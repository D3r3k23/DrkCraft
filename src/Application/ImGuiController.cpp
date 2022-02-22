#include "ImGuiController.hpp"

#include "System/AssetLibrary.hpp"
#include "Core/Debug/Profiler.hpp"

#include <imgui_impl/imgui_impl.h>

namespace DrkCraft
{
    std::unordered_map<ImGuiFont, ImFont*> ImGuiController::s_fonts;

    ImGuiController::ImGuiController(Window& window, bool enable)
      : m_enabled(enable),
        m_blockEvents(true),
        m_showDemoWindow(false)
    {
        DRK_PROFILE_FUNCTION();

        IMGUI_CHECKVERSION();
        {
            DRK_PROFILE_SCOPE("ImGui::CreateContext");
            m_context = ImGui::CreateContext();
        }
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = "data/imgui.ini";
        {
            DRK_PROFILE_SCOPE("ImGui load fonts");
            const auto fontPath = font_asset_path("Kanit-Medium.ttf");
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
        }
        setup_style();
        init_impl(window);
    }

    ImGuiController::~ImGuiController(void)
    {
        shutdown_impl();
        ImGui::DestroyContext(m_context);
    }

    void ImGuiController::init_impl(Window& window)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Initializing ImGui");

        ImGui_ImplGlfw_InitForOpenGL(window.get_raw_window(), true);
        ImGui_ImplOpenGL3_Init("#version 400");
    }

    void ImGuiController::shutdown_impl(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Shutting down ImGui");

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    }

    void ImGuiController::begin_frame(void)
    {
        DRK_PROFILE_FUNCTION();

        if (m_enabled)
        {
            reset_stats();
            {
                DRK_PROFILE_SCOPE("imgui_impl newFrame");
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
            }
            ImGui::NewFrame();
        }
    }

    void ImGuiController::end_frame(void)
    {
        DRK_PROFILE_FUNCTION();

        if (m_enabled)
        {
        #if defined(DRK_CONFIG_DEBUG)
            if (m_showDemoWindow)
                ImGui::ShowDemoWindow(&m_showDemoWindow);
        #endif
            ImGui::Render();
            ImDrawData* drawData = ImGui::GetDrawData();

            m_stats.drawLists  = drawData->CmdListsCount;
            m_stats.indices   = drawData->TotalIdxCount;
            m_stats.vertices = drawData->TotalVtxCount;

            for (uint i = 0; i < drawData->CmdListsCount; i++)
            {
                const auto& drawList = *drawData->CmdLists[i];
                m_stats.drawCmds += drawList.CmdBuffer.size();
            }{
                DRK_PROFILE_SCOPE("imgui_impl render");
                ImGui_ImplOpenGL3_RenderDrawData(drawData);
            }
        }
    }

    void ImGuiController::on_event(InputEvent& event)
    {
        if (m_enabled)
        {
            if (should_capture_event(event))
                event.set_handled();
        }
    }

    bool ImGuiController::should_capture_event(const InputEvent& event) const
    {
        if (!m_enabled || !m_blockEvents)
            return false;
        else
        {
            ImGuiIO& io = ImGui::GetIO();
            return (event == EventCategory::Mouse    && io.WantCaptureMouse)
                || (event == EventCategory::Keyboard && io.WantCaptureKeyboard);
        }
    }

    void ImGuiController::block_events(bool block)
    {
        m_blockEvents = block;
    }

    void ImGuiController::enable(void)
    {
        m_enabled = true;
    }

    void ImGuiController::disable(void)
    {
        m_enabled = false;
    }

    bool ImGuiController::enabled(void) const
    {
        return m_enabled;
    }

    void ImGuiController::enable_demo_window(void)
    {
        m_showDemoWindow = true;
    }

    void ImGuiController::disable_demo_window(void)
    {
        m_showDemoWindow = false;
    }

    void ImGuiController::toggle_demo_window(void)
    {
        m_showDemoWindow = !m_showDemoWindow;
    }

    bool ImGuiController::demo_window_enabled(void) const
    {
        return m_showDemoWindow;
    }

    const ImGuiRendererStats& ImGuiController::get_renderer_stats(void) const
    {
        return m_lastStats;
    }

    ImFont* ImGuiController::get_font(ImGuiFont font)
    {
        DRK_ASSERT_DEBUG(s_fonts.contains(font), "Unknown font");
        return s_fonts[font];
    }

    void ImGuiController::setup_style(void)
    {
        ImGui::StyleColorsDark();
    }

    void ImGuiController::reset_stats(void)
    {
        m_lastStats = m_stats;
        m_stats = ImGuiRendererStats{};
    }
}
