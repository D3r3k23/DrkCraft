#ifndef DRK_APPLICATION_HPP
#define DRK_APPLICATION_HPP

#include "Core/Base.hpp"
#include "Window.hpp"
#include "Layer.hpp"
#include "LayerStack.hpp"
#include "Timestep.hpp"
#include "MainMenu.hpp"
#include "Game/Game.hpp"

namespace DrkCraft
{
    class Application
    {
    public:
        static void init(void);
        static void start(void);
        static int  shutdown(void);

        static Application& get_instance(void);

        Application();
        ~Application();

        void init_glfw(void);

        int run(void);

        void on_update(Timestep timestep);
        void on_render(Timestep timestep);
        bool on_event(Event& event);

        bool on_window_close(WindowCloseEvent& event);
        bool on_window_resize(WindowResizeEvent& event);
        bool on_key_press(KeyPressedEvent& event);

        void open_main_menu(void);
        void close_main_menu(void);

        void start_game(void);
        void stop_game(void);

        Window& get_window(void);
        LayerStack& get_layer_stack(void);

    private:
        static Application* s_instance;
        static int s_exitCode;

        Window* m_window;

        LayerStack m_layerStack;
        LayerStack::View m_layerStackView;
        LayerStack::ReverseView m_layerStackReverseView;

        MainMenu* m_menuLayer;
        Game* m_gameLayer;

        bool m_running;
        bool m_minimized;
    };
}

#endif // DRK_APPLICATION_HPP
