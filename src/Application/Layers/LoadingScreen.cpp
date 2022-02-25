#include "LoadingScreen.hpp"

#include "Util/ImGui.hpp"
#include "Application/ImGuiController.hpp"
#include "Core/Debug/Profiler.hpp"

#include <imgui/imgui.h>
#include <fmt/format.h>

namespace DrkCraft
{
    static const uint MAX_PERIODS = 10;

    LoadingScreen::LoadingScreen(std::string_view msg, bool activate)
      : Layer("LoadingScreenLayer", activate),
        m_msg(msg),
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

    void LoadingScreen::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        DrkImGui::BeginFullscreen("Loading", ImGuiWindowFlags_NoBackground);
        ImGui::PushFont(ImGuiController::get_font(ImGuiFont::Button));

        ImGui::Dummy({100, 300});

        auto text = fmt::format("{}{:.>{}}{: >{}}", "", m_msg, m_numPeriods, "", MAX_PERIODS - m_numPeriods);
        DrkImGui::TextCentered(text);

        ImGui::PopFont();
        ImGui::End();
    }

    void LoadingScreen::on_update(Timestep timestep)
    {
        if (m_updateTimer.on_interval<250>())
        {
            m_numPeriods++;
            if (m_numPeriods > MAX_PERIODS)
                m_numPeriods = 0;
        }
    }

    void LoadingScreen::on_event(Event& event)
    {

    }

    void LoadingScreen::set_message(std::string_view msg)
    {
        m_msg = msg;
    }
}
