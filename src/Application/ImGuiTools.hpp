#ifndef DRK_IMGUI_TOOLS_HPP
#define DRK_IMGUI_TOOLS_HPP

#include "Core/Base.hpp"
#include "Events.hpp"

#include <imgui/imgui.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

namespace DrkCraft
{
    enum class Font
    {
        Regular = 0,
        Title
    };

    class ImGuiManager
    {
    public:
        ImGuiManager(GLFWwindow* window, bool enable=true);
        ~ImGuiManager(void);

        void begin_frame(void);
        void end_frame(void);

        void on_event(Event& event);

        void enable(void);
        void disable(void);

        void block_events(bool block);

        static ImFont* get_font(Font font);

    private:
        void setup_style(void);

        bool on_key_pressed(const KeyPressedEvent& event);

    private:
        bool m_enabled; // What happens if this is false and we still try and use ImGui?
        bool m_blockEvents;

        bool m_showDemoWindow;

        static std::unordered_map<Font, ImFont*> s_fonts;
    };

    namespace ImGuiTools
    {
        void BeginFullscreen(std::string_view name, ImGuiWindowFlags flags=0);
        void BeginCentered(std::string_view name, const ImVec2& size, ImGuiWindowFlags flags=0);
        void TextCentered(std::string_view text);
        bool ButtonCentered(const char* text, const ImVec2& size);
    }
}

#endif // DRK_IMGUI_TOOLS_HPP
