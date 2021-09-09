#include "Base.hpp"
#include "Application.hpp"

int main(int, char*[])
{
    DrkCraft::Log::init();

    LOG(Info, "Creating application");
    auto app = DrkCraft::App::create();

    LOG(Info, "Running application");
    int returnCode = app->run();
    if (returnCode)
        LOG(Error, "Application closed with error code {returnCode}");

    LOG(Info, "Shutting down application");
    delete app;
}
