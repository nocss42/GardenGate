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
        GG::Log::init();

        GG_LOG(GG::LogLevel::Info, "%.*s", (int)GG::Version::Banner.size(), GG::Version::Banner.data());
        GG_LOG(GG::LogLevel::Info, "Git commit: %.*s", (int)GG::Version::GitInfo.size(), GG::Version::GitInfo.data());
        GG_LOG(GG::LogLevel::Info, GG_CONSOLE_TITLE);
    }

    void uninitialize()
    {
        GG::Log::shutdown();
        if (GG::Log::detail::g_console_enabled)
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
    std::atomic<bool> m_running{ true };
};

inline std::unique_ptr<Program> g_program = nullptr;
