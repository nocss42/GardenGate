#pragma once
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <source_location>
#include <string>
#include <string_view>
#include <vector>

#ifndef NOMINMAX
    #define NOMINMAX
#endif
#define _WINSOCKAPI_
#include <windows.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Platform.h"

namespace GG
{
enum class LogLevel : unsigned char
{
    DebugPlusPlus = 0,
    Debug,
    Info,
    Warning,
    Error,
    Fatal,

#if defined(_DEBUG)
    Current = Debug
#else
    Current = Info
#endif
};

namespace Log
{
namespace detail
{
#if defined(_DEBUG)
inline constexpr bool IsDebugBuild = true;
#else
inline constexpr bool IsDebugBuild = false;
#endif

inline std::shared_ptr<spdlog::logger> g_logger;
inline bool g_console_enabled = false;

[[nodiscard]] inline spdlog::level::level_enum to_spdlog(LogLevel lvl) noexcept
{
    switch (lvl)
    {
    case LogLevel::DebugPlusPlus:
        return spdlog::level::trace;
    case LogLevel::Debug:
        return spdlog::level::debug;
    case LogLevel::Info:
        return spdlog::level::info;
    case LogLevel::Warning:
        return spdlog::level::warn;
    case LogLevel::Error:
        return spdlog::level::err;
    case LogLevel::Fatal:
        return spdlog::level::critical;
    default:
        return spdlog::level::info;
    }
}

[[nodiscard]] inline LogLevel effective_log_level() noexcept
{
    static const LogLevel level = []() noexcept -> LogLevel {
        char env[16]{};
        DWORD len = GetEnvironmentVariableA("GG_LOG_LEVEL", env, sizeof(env));
        if (!len)
            return LogLevel::Current;
        while (len > 0 && env[len - 1] == ' ')
            env[--len] = '\0';
        if (_stricmp(env, "Debug++") == 0)
            return LogLevel::DebugPlusPlus;
        if (_stricmp(env, "Debug") == 0)
            return LogLevel::Debug;
        if (_stricmp(env, "Info") == 0)
            return LogLevel::Info;
        if (_stricmp(env, "Warning") == 0)
            return LogLevel::Warning;
        if (_stricmp(env, "Error") == 0)
            return LogLevel::Error;
        if (_stricmp(env, "Fatal") == 0)
            return LogLevel::Fatal;
        return LogLevel::Current;
    }();
    return level;
}

[[nodiscard]] inline bool should_log(LogLevel lvl) noexcept
{
    return static_cast<unsigned>(lvl) >= static_cast<unsigned>(effective_log_level());
}

[[nodiscard]] inline std::shared_ptr<spdlog::logger> make_logger(std::string_view log_name)
{
    std::vector<spdlog::sink_ptr> sinks;

    if (!log_name.empty())
    {
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(std::string(log_name), false);
        sinks.push_back(std::move(file_sink));
    }

    if (g_console_enabled)
    {
        auto con_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        sinks.push_back(std::move(con_sink));
    }

    if (sinks.empty())
        return nullptr;

    const std::string name = std::string(GG::Version::AppName) + ":" + std::string(GG::Version::BuildName);
    auto logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());

    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);

    if constexpr (IsDebugBuild)
        logger->set_pattern("%^[%d-%m-%Y %H:%M:%S] [%n] [%l]%$ [%s:%#] %v");
    else
        logger->set_pattern("%^[%d-%m-%Y %H:%M:%S] [%n] [%l]%$ %v");

    return logger;
}

[[noreturn]] inline void hard_abort() noexcept
{
    if (g_logger)
        g_logger->flush();
    std::fflush(stdout);
#if defined(_MSC_VER) && defined(_DEBUG)
    __debugbreak();
#endif
    std::abort();
}

inline void platform_fatal_dialog(const char* message) noexcept
{
    ::MessageBoxA(nullptr, message, "Fatal Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
}
}

inline void init()
{
    {
        char env[2]{};
        detail::g_console_enabled = GetEnvironmentVariableA("GG_CONSOLE", env, sizeof(env)) > 0;
    }

    if (detail::g_console_enabled)
    {
        AllocConsole();
        SetConsoleTitleA(GG_CONSOLE_TITLE);
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(hOut, &mode);
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        SetConsoleOutputCP(CP_UTF8);
    }

    // Always create the console sink (on Wine/Linux stdout goes to terminal anyway)
    detail::g_console_enabled = true;

    char log_name[256]{};
    DWORD log_len = GetEnvironmentVariableA("GG_LOG", log_name, sizeof(log_name));
    while (log_len > 0 && log_name[log_len - 1] == ' ')
        log_name[--log_len] = '\0';

    std::string log_path;
    if (log_len > 0)
    {
        std::time_t now = std::time(nullptr);
        std::tm tm{};
        localtime_s(&tm, &now);
        char ts[32]{};
        std::strftime(ts, sizeof(ts), "%Y%m%d_%H%M%S", &tm);
        log_path = std::string(log_name) + "." + ts + ".log";
    }
    detail::g_logger = detail::make_logger(log_path);
}

inline void shutdown()
{
    if (detail::g_logger)
        detail::g_logger->flush();
    spdlog::shutdown();
}

[[nodiscard]] inline bool should_log(LogLevel lvl) noexcept
{
    return detail::should_log(lvl);
}
}

template<class... Args>
inline void logf(LogLevel lvl, const std::source_location& loc, const char* fmt, Args&&... args) noexcept
{
    if (!Log::detail::g_logger || !Log::detail::should_log(lvl))
        return;

    char buf[8192]{};
    std::snprintf(buf, sizeof(buf), fmt, std::forward<Args>(args)...);

    if constexpr (Log::detail::IsDebugBuild)
    {
        Log::detail::g_logger->log(
            spdlog::source_loc{ loc.file_name(), static_cast<int>(loc.line()), loc.function_name() }, Log::detail::to_spdlog(lvl), buf);
    }
    else
    {
        (void)loc;
        Log::detail::g_logger->log(Log::detail::to_spdlog(lvl), buf);
    }
}

template<class... Args>
[[noreturn]] inline void fatalf(const std::source_location& loc, const char* fmt, Args&&... args) noexcept
{
    char buf[2048]{};
    std::snprintf(buf, sizeof(buf), fmt, std::forward<Args>(args)...);

    logf(LogLevel::Fatal, loc, "%s", buf);
    Log::detail::platform_fatal_dialog(buf);
    Log::detail::hard_abort();
}
}

#define GG_LOG(level, fmt, ...) ::GG::logf((level), std::source_location::current(), (fmt), ##__VA_ARGS__)
#define GG_FATAL(fmt, ...) ::GG::fatalf(std::source_location::current(), (fmt), ##__VA_ARGS__)
