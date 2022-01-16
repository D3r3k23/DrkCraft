#include "GlfwTools.hpp"

#include "Core/BuildSettings.hpp"
#include "Core/Profiler.hpp"

#include <GLFW/glfw3.h>

namespace DrkCraft
{
    void init_glfw(void)
    {
        DRK_PROFILE_FUNCTION();
        {
            DRK_PROFILE_SCOPE("glfwInit");
            auto status = glfwInit();
            DRK_ASSERT_CORE(status == GLFW_TRUE, "Failed to initialize GLFW");
        }
        if constexpr (DRK_LOGGING_ENABLED)
        {
            glfwSetErrorCallback([](int error, const char* description)
            {
                DRK_LOG_CORE_ERROR("GLFW Error [{}]: {}", error, description);
            });
        }
    }

    void shutdown_glfw(void)
    {
        DRK_PROFILE_FUNCTION();
        glfwTerminate();
    }
}
