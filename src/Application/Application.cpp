#include "Application.hpp"

#include "Graphics/Renderer.hpp"
#include "Core/Profiler.hpp"

#include <GLFW/glfw3.h>

namespace DrkCraft
{
    Application* Application::s_instance = nullptr;

    void Application::init(void)
    {
        s_instance = new Application;
    }

    int Application::shutdown(void)
    {
        if (s_instance)
        {
            int exitCode = s_instance->m_exitCode;
            delete s_instance;
            if (exitCode)
                DRK_LOG_ERROR("Application stopped with error code {}", exitCode);
            return exitCode;
        }
        else
        {
            DRK_LOG_WARN("Application is already shutdown");
            return 1;
        }
    }

    Application& Application::get_instance(void)
    {
        DRK_ASSERT_DEBUG(s_instance, "Application not initialized");
        return *s_instance;
    }

    Application::Application(void)
      : m_exitCode(0),
        m_running(false),
        m_minimized(false)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_TRACE("Initializing GLFW");
        init_glfw();

        DRK_LOG_TRACE("Creating Window");
        m_window = new Window("DrkCraft", 1280, 720, true);

        DRK_LOG_TRACE("Registering event handler");
        m_window->register_event_handler(DRK_BIND_FN(on_event));

        DRK_LOG_TRACE("Initializing Renderer");
        Renderer::init();

        DRK_LOG_TRACE("Creating ImGuiManager");
        m_imGuiManager = new ImGuiManager;
    }

    Application::~Application(void)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_TRACE("Clearing Layer Stack");
        m_layerStack.clear();

        DRK_LOG_TRACE("Deleting ImGuiManger");
        delete m_imGuiManager;

        DRK_LOG_TRACE("Shutting down Renderer");
        Renderer::shutdown();

        DRK_LOG_TRACE("Deleting Window");
        delete m_window;

        DRK_LOG_TRACE("Terminating GLFW");
        glfwTerminate();
    }

    void Application::init_glfw(void)
    {
        auto status = glfwInit();
        DRK_ASSERT_CORE(status == GLFW_TRUE, "Failed to initialize GLFW");

    #if defined(DRK_EN_LOGGING)
        glfwSetErrorCallback([](int error, const char* description)
        {
            DRK_LOG_ERROR("GLFW Error [{}]: {}", error, description);
        });
    #endif
    }

    void Application::add_overlay(const Ref<Layer>& layer)
    {
        layer->attach_layer();
        m_layerStack.push_front(layer);
    }

    void Application::add_layer(const Ref<Layer>& layer)
    {
        layer->attach_layer();
        m_layerStack.push_back(layer);
    }

    void Application::run(void)
    {
        DRK_PROFILE_FUNCTION();

        m_running = true;
        while (m_running)
        {
            Timestep timestep;
            {
                DRK_PROFILE_SCOPE("LayerStackView-creation");
                LayerStack frameLayerStack = m_layerStack;
                m_layerStackForwardView = make_ptr<LayerStack::ForwardView>(frameLayerStack);
                m_layerStackReverseView = make_ptr<LayerStack::ReverseView>(frameLayerStack);
            }
            m_window->on_update();

            if (m_running && !m_minimized)
            {
                on_update(timestep);
                on_render(timestep);
            }

            m_layerStackForwardView.reset();
            m_layerStackReverseView.reset();

            m_layerStack.refresh();

            if (m_layerStack.is_empty())
            {
                DRK_LOG_INFO("LayerStack is empty; exiting Application");
                exit();
            }
            else if (m_layerStack.all_layers_inactive())
            {
                DRK_LOG_WARN("LayerStack is not empty, but there are no active layers");
                DRK_LOG_WARN("Reactivating back layer");
                m_layerStack.activate_back();
            }
        }
    }

    void Application::exit(int status)
    {
        m_running  = false;
        m_exitCode = status;
    }

    void Application::on_update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        for (auto& layer : *m_layerStackReverseView)
            if (layer->is_layer_active())
                layer->on_update(timestep);
    }

    void Application::on_render(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        Renderer::begin_frame();
        m_imGuiManager->begin_frame();

        for (auto& layer : *m_layerStackReverseView)
            if (layer->is_layer_active())
                layer->on_render(timestep);

        m_imGuiManager->end_frame();
        Renderer::end_frame();
    }

    void Application::on_event(Event& event)
    {
        DRK_PROFILE_FUNCTION();

        log_event(event);

        EventDispatcher ed(event);
        ed.dispatch<WindowCloseEvent>(DRK_BIND_FN(on_window_close));
        ed.dispatch<WindowResizeEvent>(DRK_BIND_FN(on_window_resize));
        ed.dispatch<FramebufferResizeEvent>(DRK_BIND_FN(on_framebuffer_resize));

        m_imGuiManager->on_event(event);

        for (auto& layer : *m_layerStackForwardView)
            if (layer->is_layer_active())
                layer->on_event(event);

        // if (event.handled())
        //     return true;
        // else
        // {
        //     Or just log here and specify if handled?
        //     DRK_LOG_WARN("Event: {} not handled", std::string(event));
        //     return false;
        // }
    }

    bool Application::on_window_close(WindowCloseEvent& event)
    {
        exit();
        return true;
    }

    bool Application::on_window_resize(WindowResizeEvent& event)
    {
        m_window->resize(event.width, event.height);
        return true;
    }

    bool Application::on_framebuffer_resize(FramebufferResizeEvent& event)
    {
        Renderer::set_viewport(0, 0, event.width, event.height);
        return true;
    }

    Window& Application::get_window(void)
    {
        DRK_ASSERT_DEBUG(m_window, "Window not initialized");
        return *m_window;
    }
}
