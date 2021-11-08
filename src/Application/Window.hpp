#ifndef DRK_WINDOW_HPP
#define DRK_WINDOW_HPP

#include "Core/Base.hpp"
#include "EventGenerator.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <string>

namespace DrkCraft
{
    class Window
    {
    public:
        Window(std::string_view name, uint width, uint height);
        ~Window(void);

        void register_event_handler(const EventHandlerFn& handler);

        void on_update(void);

        GLFWwindow* get_native_window(void) const;

        glm::uvec2 resize(uint width, uint height);
        glm::uvec2 resize(glm::uvec2 size);

        glm::uvec2 get_size(void) const;
        glm::uvec2 get_framebuffer_size(void) const; // ???

    private:
        GLFWwindow* m_window;
        std::string m_title;

        EventGenerator eventGenerator;
    };
}

#endif // DRK_WINDOW_HPP
