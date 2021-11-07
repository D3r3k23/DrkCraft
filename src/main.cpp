#include "Core/Base.hpp"
#include "Application/Application.hpp"

using namespace DrkCraft;

int main(int, char**)
{
    DRK_LOGGER_INIT("DrkCraft");

    Application::init();
    Application::start();
    int status = Application::shutdown();

    DRK_LOGGER_SAVE();
    return status;
}
