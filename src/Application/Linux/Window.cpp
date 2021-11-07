#include "../Window.hpp"

#if defined(DRK_PLATFORM_LINUX)
    namespace DrkCraft
    {
        Window::Window(std::string_view name, uint width, uint height)
          : m_name(name)
        {

        }
    }
#endif
