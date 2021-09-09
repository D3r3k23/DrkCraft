#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Base.hpp"
#include "Window.hpp"

namespace DrkCraft
{
    class App
    {
    public:
        App();
        ~App();

        static App* create() { return new App; }

        int run(void);

    private:
        Ptr<Window> m_window;
    };
}

#endif // APPLICATION_HPP
