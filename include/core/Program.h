#pragma once

#include <atomic>
#include <memory>

#include "base/Log.h"
#include "base/Platform.h"

class Program
{
public:
    void initialize()
    {
        AllocConsole();
        SetConsoleTitleA(GG_CONSOLE_TITLE);
        freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(hOut, &mode);
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        SetConsoleOutputCP(CP_UTF8);

        std::cout << GG::Version::Banner << std::endl;
        GG_LOG(GG::LogLevel::Info, GG_CONSOLE_TITLE);
    }

    void uninitialize()
    {
        fclose((FILE *)stdout);
        FreeConsole();
    }

    void run()
    {
        while (m_running.load())
        {
            if (GetAsyncKeyState(VK_F12) & 1)
            {
                GG_LOG(GG::LogLevel::Info, "F12 pressed, stopping program");
                m_running.store(false);
            }

            ::Sleep(1);
        }
    }

    void stop()
    {
        m_running.store(false);
    }

private:
    std::atomic<bool> m_running{true};
};

inline std::unique_ptr<Program> g_program = nullptr;
