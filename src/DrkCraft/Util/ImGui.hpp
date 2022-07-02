#ifndef DRK_UTIL_IMGUI_HPP
#define DRK_UTIL_IMGUI_HPP

#include "Core/Base.hpp"

#include "Lib/string_view.hpp"

#include <imgui/imgui.h>

namespace DrkCraft::DrkImGui // Change this
{
    void BeginFullscreen(string_view name, ImGuiWindowFlags flags=0);
    void BeginCentered(string_view name, const ImVec2& size, ImGuiWindowFlags flags=0);
    void TextCentered(string_view text);
    bool ButtonCentered(const char* text, const ImVec2& size);
}

#endif // DRK_UTIL_IMGUI_HPP
