#pragma once
#include <cstdio>
#include <cstdlib>
#include <source_location>
#include <string_view>
#include <utility>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#define _WINSOCKAPI_
#include <windows.h>

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

    namespace detail
    {
#if defined(_DEBUG)
        inline constexpr bool IsDebugBuild = true;
#else
        inline constexpr bool IsDebugBuild = false;
#endif

        inline constexpr std::string_view Reset = "\x1b[0m";

        [[nodiscard]] inline constexpr std::string_view LOG_LEVEL_TO_STRING(LogLevel lvl) noexcept
        {
            switch (lvl)
            {
            case LogLevel::DebugPlusPlus:
                return "Debug++";
            case LogLevel::Debug:
                return "Debug";
            case LogLevel::Info:
                return "Info";
            case LogLevel::Warning:
                return "Warning";
            case LogLevel::Error:
                return "Error";
            case LogLevel::Fatal:
                return "Fatal";
            default:
                return "Unknown";
            }
        }

        [[nodiscard]] inline constexpr std::string_view LOG_LEVEL_COLOR(LogLevel lvl) noexcept
        {
            switch (lvl)
            {
            case LogLevel::DebugPlusPlus:
            case LogLevel::Debug:
                return "\x1b[36m";
            case LogLevel::Info:
                return "";
            case LogLevel::Warning:
                return "\x1b[33m";
            case LogLevel::Error:
            case LogLevel::Fatal:
                return "\x1b[31m";
            default:
                return "";
            }
        }

        [[nodiscard]] inline constexpr bool should_log(LogLevel lvl) noexcept
        {
            return static_cast<unsigned>(lvl) >= static_cast<unsigned>(LogLevel::Current);
        }

        [[nodiscard]] inline constexpr std::string_view filename(std::string_view path) noexcept
        {
            const auto slash = path.find_last_of("/\\");
            return (slash == std::string_view::npos) ? path : path.substr(slash + 1);
        }

        inline void print_header(LogLevel lvl, const std::source_location &loc) noexcept
        {
            std::printf("%.*s[%.*s:%.*s] [%.*s]",
                        (int)LOG_LEVEL_COLOR(lvl).size(), LOG_LEVEL_COLOR(lvl).data(),
                        (int)GG::Version::AppName.size(), GG::Version::AppName.data(),
                        (int)GG::Version::BuildName.size(), GG::Version::BuildName.data(),
                        (int)LOG_LEVEL_TO_STRING(lvl).size(), LOG_LEVEL_TO_STRING(lvl).data());

            if constexpr (IsDebugBuild)
            {
                const auto fn = filename(loc.file_name());
                std::printf(" [%.*s:%u] ", (int)fn.size(), fn.data(), loc.line());
            }
            else
            {
                (void)loc;
                std::printf(" ");
            }
        }

        template <class... Args>
        inline void vlogf(LogLevel lvl,
                          const std::source_location &loc,
                          const char *fmt,
                          Args &&...args) noexcept
        {
            if (!should_log(lvl))
                return;

            print_header(lvl, loc);
            std::printf(fmt, std::forward<Args>(args)...);
            std::printf("%.*s\n", (int)Reset.size(), Reset.data());
            std::fflush(stdout);
        }

        inline void platform_fatal_dialog(const char *message) noexcept
        {
            ::MessageBoxA(nullptr, message, "Fatal Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        }

        [[noreturn]] inline void hard_abort() noexcept
        {
            std::fflush(stdout);
#if defined(_MSC_VER) && defined(_DEBUG)
            __debugbreak();
#endif
            std::abort();
        }
    }

    template <class... Args>
    inline void logf(LogLevel lvl,
                     const std::source_location &loc,
                     const char *fmt,
                     Args &&...args) noexcept
    {
        detail::vlogf(lvl, loc, fmt, std::forward<Args>(args)...);
    }

    template <class... Args>
    [[noreturn]] inline void fatalf(const std::source_location &loc,
                                    const char *fmt,
                                    Args &&...args) noexcept
    {
        char buf[2048]{};
        std::snprintf(buf, sizeof(buf), fmt, std::forward<Args>(args)...);

        detail::vlogf(LogLevel::Fatal, loc, "%s", buf);
        detail::platform_fatal_dialog(buf);
        detail::hard_abort();
    }
}

#define GG_LOG(level, fmt, ...) \
    ::GG::logf((level), std::source_location::current(), (fmt), ##__VA_ARGS__)

#define GG_FATAL(fmt, ...) \
    ::GG::fatalf(std::source_location::current(), (fmt), ##__VA_ARGS__)
