#ifndef DRK_UTIL_IMGUI_HPP
#define DRK_UTIL_IMGUI_HPP

#include "Core/Base.hpp"

#include <imgui/imgui.h>

#include <string_view>

namespace DrkCraft::DrkImGui
{
    void BeginFullscreen(std::string_view name, ImGuiWindowFlags flags=0);
    void BeginCentered(std::string_view name, const ImVec2& size, ImGuiWindowFlags flags=0);
    void TextCentered(std::string_view text);
    bool ButtonCentered(const char* text, const ImVec2& size);
}

#endif // DRK_UTIL_IMGUI_HPP
