#include "LoadingScreen.hpp"

#include "Util/ImGui.hpp"
#include "Application/ImGuiController.hpp"
#include "System/Lock.hpp"
#include "Core/Debug/Profiler.hpp"

#include "Lib/string_view.hpp"

#include <imgui/imgui.h>
#include <fmt/format.h>

#include <utility>

namespace DrkCraft
{
    static const uint MAX_PERIODS = 10;

    LoadingScreen::LoadingScreen(string msg, bool activate)
      : Layer("LoadingScreenLayer", activate),
        m_msg(std::move(msg)),
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

        string_view msg = [&]
        {
            Lock<> lock(m_msgMutex);
            return m_msg;
        }();
        auto text = fmt::format("{}{:.>{}}{: >{}}", "", msg, m_numPeriods, "", MAX_PERIODS - m_numPeriods);
        DrkImGui::TextCentered(text);

        ImGui::PopFont();
        ImGui::End();
    }

    void LoadingScreen::on_update(Timestep timestep)
    {
        if (m_updateTimer.on_interval<250>())
        {
            ++m_numPeriods;
            if (m_numPeriods > MAX_PERIODS)
                m_numPeriods = 0;
        }
    }

    void LoadingScreen::on_event(Event& event)
    {

    }

    void LoadingScreen::set_message(string msg)
    {
        Lock<> lock(m_msgMutex);
        m_msg = std::move(msg);
    }
}
