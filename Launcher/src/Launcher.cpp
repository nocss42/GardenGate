#include "Launcher.hpp"
#include "Renderer.hpp"
#include "config/Config.hpp"
#include "utils/Utils.hpp"

#include <fstream>
#include <iostream>

Config load_config(const std::string& file);
void save_config(const Config& cfg, const std::string& file);

Config g_config;

int Launcher::Run()
{
    if (Utils::Process::handleCLI())
    {
        return 0;
    }

    g_config = load_config("config.json");

    Renderer::Run();
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return Launcher::Run();
}