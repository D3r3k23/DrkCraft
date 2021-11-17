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
                DRK_LOG_CORE_ERROR("Application stopped with error code {}", exitCode);
            return exitCode;
        }
        else
        {
            DRK_LOG_CORE_WARN("Application is already shutdown");
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

        DRK_LOG_CORE_TRACE("Initializing GLFW");
        init_glfw();

        DRK_LOG_CORE_TRACE("Creating Window");
        m_window = new Window("DrkCraft");

        DRK_LOG_CORE_TRACE("Registering event handler");
        m_window->register_event_handler(DRK_BIND_FN(on_event));

        DRK_LOG_CORE_TRACE("Initializing Renderer");
        Renderer::init();

        DRK_LOG_CORE_TRACE("Creating ImGuiManager");
        m_imGuiManager = new ImGuiManager(m_window->get_raw_window());
    }

    Application::~Application(void)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Clearing Layer Stack");
        m_layerStack.clear();

        DRK_LOG_CORE_TRACE("Deleting ImGuiManger");
        delete m_imGuiManager;

        DRK_LOG_CORE_TRACE("Shutting down Renderer");
        Renderer::shutdown();

        DRK_LOG_CORE_TRACE("Deleting Window");
        delete m_window;

        DRK_LOG_CORE_TRACE("Terminating GLFW");
        glfwTerminate();
    }

    void Application::init_glfw(void)
    {
        auto status = glfwInit();
        DRK_ASSERT_CORE(status == GLFW_TRUE, "Failed to initialize GLFW");

    #if defined(DRK_EN_LOGGING)
        glfwSetErrorCallback([](int error, const char* description)
        {
            DRK_LOG_CORE_ERROR("GLFW Error [{}]: {}", error, description);
        });
    #endif
    }

    Window& Application::get_window(void)
    {
        DRK_ASSERT_DEBUG(m_window, "Window not initialized");
        return *m_window;
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
            LayerStack frameLayerStack;
            {
                DRK_PROFILE_SCOPE("LayerStackView creation");
                frameLayerStack = LayerStack::copy_active(m_layerStack);
                m_layerStackForwardView = make_ptr<LayerStack::ForwardView>(frameLayerStack);
                m_layerStackReverseView = make_ptr<LayerStack::ReverseView>(frameLayerStack);
            }{
                DRK_PROFILE_SCOPE("Application core loop");

                m_window->swap_buffers();
                m_window->poll_events();

                if (m_running && !m_minimized)
                {
                    update(timestep);
                    render();
                }
            }
            m_layerStackForwardView.reset();
            m_layerStackReverseView.reset();

            m_layerStack.refresh();

            if (m_layerStack.empty())
            {
                DRK_LOG_CORE_INFO("LayerStack is empty; exiting Application");
                exit();
            }
            else if (m_layerStack.all_layers_inactive())
            {
                DRK_LOG_CORE_WARN("LayerStack is not empty, but there are no active layers");
                DRK_LOG_CORE_WARN("Reactivating back layer");
                m_layerStack.activate_back();
            }
        }
    }

    void Application::exit(int status)
    {
        m_running  = false;
        m_exitCode = status;
    }

    void Application::update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        for (auto& layer : *m_layerStackReverseView)
            layer->on_update(timestep);
    }

    void Application::render(void)
    {
        DRK_PROFILE_FUNCTION();

        Renderer::begin_frame();
        m_imGuiManager->begin_frame();
        {
            DRK_PROFILE_SCOPE("Render Layers");
            for (auto& layer : *m_layerStackReverseView)
                layer->on_render();
        }
        m_imGuiManager->end_frame();
        Renderer::end_frame();
    }

    void Application::on_event(Event& event)
    {
        DRK_PROFILE_FUNCTION();

        EventDispatcher ed(event);
        ed.dispatch<WindowClosedEvent>(DRK_BIND_FN(on_window_closed));
        ed.dispatch<FramebufferResizedEvent>(DRK_BIND_FN(on_framebuffer_resized));

        // We could redraw the screen on resize/move

        m_imGuiManager->on_event(event);

        for (auto& layer : *m_layerStackForwardView)
            layer->on_event(event);

        DRK_LOG_EVENT(event);
    }

    bool Application::on_window_closed(const WindowClosedEvent& event)
    {
        exit();
        return true;
    }

    bool Application::on_framebuffer_resized(const FramebufferResizedEvent& event)
    {
        Renderer::set_viewport(0, 0, event.width, event.height);
        return true;
    }
}
