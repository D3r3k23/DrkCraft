#include "ImGui.hpp"

#include "Core/Debug/Profiler.hpp"

namespace DrkCraft::DrkImGui
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
