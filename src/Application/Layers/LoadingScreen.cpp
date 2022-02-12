#include "LoadingScreen.hpp"

#include "Application/ImGuiTools.hpp"
#include "Core/Debug/Profiler.hpp"

#include <imgui/imgui.h>
#include <fmt/format.h>

namespace DrkCraft
{
    static const uint MAX_PERIODS = 10;

    LoadingScreen::LoadingScreen(bool activate)
      : Layer("LoadingScreenLayer", activate),
        m_updateTimer(250),
        m_numPeriods(0)
    { }

    LoadingScreen::~LoadingScreen(void)
    {

    }

    void LoadingScreen::on_attach(void)
    {

    }

    void LoadingScreen::on_detach(void)
    {

    }

    void LoadingScreen::on_update(Timestep timestep)
    {
        if (m_updateTimer.on_interval())
        {
            m_numPeriods++;
            if (m_numPeriods > MAX_PERIODS)
                m_numPeriods = 0;
        }
    }

    void LoadingScreen::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        ImGuiTools::BeginFullscreen("Loading", ImGuiWindowFlags_NoBackground);
        ImGui::PushFont(ImGuiManager::get_font(ImGuiFont::Button));

        ImGui::Dummy({100, 300});

        auto text = fmt::format("Loading{:.>{}}{: >{}}", "", m_numPeriods, "", MAX_PERIODS - m_numPeriods);
        ImGuiTools::TextCentered(text);

        ImGui::PopFont();
        ImGui::End();
    }

    void LoadingScreen::on_event(Event& event)
    {

    }
}
