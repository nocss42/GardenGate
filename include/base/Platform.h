#pragma once
#include <string_view>

#ifndef GG_VERSION_MAJOR
#define GG_VERSION_MAJOR 0
#endif

#ifndef GG_VERSION_MINOR
#define GG_VERSION_MINOR 1
#endif

#ifndef GG_VERSION_PATCH
#define GG_VERSION_PATCH 2
#endif

#define GG_STRINGIFY_IMPL(x) #x
#define GG_STRINGIFY(x) GG_STRINGIFY_IMPL(x)

#if defined(_DEBUG)
#define GG_BUILD_NAME "Debug"
#else
#define GG_BUILD_NAME "Release"
#endif

#define GG_VERSION_STRING          \
    GG_STRINGIFY(GG_VERSION_MAJOR) \
    "." GG_STRINGIFY(GG_VERSION_MINOR) "." GG_STRINGIFY(GG_VERSION_PATCH)

#ifdef GG_VERSION_TAG
#define GG_VERSION_FULL_STRING GG_VERSION_STRING "-" GG_VERSION_TAG
#else
#define GG_VERSION_FULL_STRING GG_VERSION_STRING
#endif

#define GG_APP_NAME "GardenGate"
#define GG_CONSOLE_TITLE GG_APP_NAME " v" GG_VERSION_FULL_STRING " (" GG_BUILD_NAME ")"

namespace GG::Version
{
    inline constexpr std::string_view AppName = GG_APP_NAME;
    inline constexpr std::string_view BuildName = GG_BUILD_NAME;
    inline constexpr std::string_view Version = GG_VERSION_STRING;
    inline constexpr std::string_view VersionFull = GG_VERSION_FULL_STRING;
    inline constexpr std::string_view ConsoleTitle = GG_CONSOLE_TITLE;
    inline constexpr std::string_view Banner = R"(
░██████╗░░█████╗░██████╗░██████╗░███████╗███╗░░██╗░██████╗░░█████╗░████████╗███████╗
██╔════╝░██╔══██╗██╔══██╗██╔══██╗██╔════╝████╗░██║██╔════╝░██╔══██╗╚══██╔══╝██╔════╝
██║░░██╗░███████║██████╔╝██║░░██║█████╗░░██╔██╗██║██║░░██╗░███████║░░░██║░░░█████╗░░
██║░░╚██╗██╔══██║██╔══██╗██║░░██║██╔══╝░░██║╚████║██║░░╚██╗██╔══██║░░░██║░░░██╔══╝░░
╚██████╔╝██║░░██║██║░░██║██████╔╝███████╗██║░╚███║╚██████╔╝██║░░██║░░░██║░░░███████╗
░╚═════╝░╚═╝░░╚═╝╚═╝░░╚═╝╚═════╝░╚══════╝╚═╝░░╚══╝░╚═════╝░╚═╝░░╚═╝░░░╚═╝░░░╚══════╝
)";
}
