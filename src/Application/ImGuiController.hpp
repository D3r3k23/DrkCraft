#ifndef DRK_UTIL_IMGUI_CONTROLLER_HPP
#define DRK_UTIL_IMGUI_CONTROLLER_HPP

#include "Core/Base.hpp"
#include "System/Window.hpp"
#include "Application/Events.hpp"

#include <imgui/imgui.h>

#include <unordered_map>

namespace DrkCraft
{
    enum class ImGuiFont
    {
        Regular = 0,
        Button,
        Title
    };

    // IMfont* to_imgui_font(const Font& font);
    // void* to_imgui_image(const Texture& texture);
    // void* to_imgui_image(const Image& image); // to_imgui_image(Texture::create(image));

    struct ImGuiRendererStats
    {
        uint drawCmds  = 0;
        uint drawLists = 0;
        uint indices   = 0;
        uint vertices  = 0;
    };

    class ImGuiController
    {
    public:
        ImGuiController(Window& window, bool enable=true);
        ~ImGuiController(void);

        void init_impl(Window& window);
        void shutdown_impl(void);

        void begin_frame(void);
        void end_frame(void);

        void on_event(InputEvent& event);

        bool should_capture_event(const InputEvent& event) const;
        void block_events(bool block);

        void enable(void);
        void disable(void);
        bool enabled(void) const;

        void enable_demo_window(void);
        void disable_demo_window(void);
        void toggle_demo_window(void);
        bool demo_window_enabled(void) const;

        const ImGuiRendererStats& get_renderer_stats(void) const;

        static ImFont* get_font(ImGuiFont font); // Fix this

    private:
        void setup_style(void);
        void reset_stats(void);

    private:
        ImGuiContext* m_context;
        bool m_enabled; // What happens if this is false and we still try and use ImGui?
        bool m_blockEvents;

        ImGuiRendererStats m_stats;
        ImGuiRendererStats m_lastStats;

        bool m_showDemoWindow;

        static std::unordered_map<ImGuiFont, ImFont*> s_fonts;
    };
}

#endif // DRK_UTIL_IMGUI_HPP
