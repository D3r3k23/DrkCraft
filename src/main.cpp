#include "Core/Base.hpp"
#include "Application/Application.hpp"

namespace DrkCraft
{
    int main(int, char*[])
    {
        Log::init();

        LOG(INFO, "Creating application");
        Application app;

        LOG(INFO, "Running application");
        int returnCode = app.run();

        if (returnCode)
            LOG(ERROR, "Application stopped with error code {returnCode}");

        LOG(INFO, "Shutting down application");
        delete app;

        Log::save();
    }
}
