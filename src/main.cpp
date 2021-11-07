#include "Core/Base.hpp"
#include "Application/Application.hpp"

using namespace DrkCraft;

int main(int, char**)
{
    DRK_LOGGER_INIT("DrkCraft");

    DRK_LOG_INFO("Creating application");
    auto app = new Application;

    DRK_LOG_INFO("Running application");
    int returnCode = app->run();

    if (returnCode)
        DRK_LOG_ERROR("Application stopped with error code {}", returnCode);

    DRK_LOG_INFO("Shutting down application");
    delete app;

    DRK_LOGGER_SAVE();
    return returnCode;
}
