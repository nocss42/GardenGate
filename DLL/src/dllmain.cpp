#include <iostream>
#include <memory>
#include <string>
#include <string_view>

#include <winsock2.h>
#define _WINSOCKAPI_
#include <windows.h>

#include "core/Program.h"
#include "core/Game.h"
#include "core/HookManager.h"
#include "core/Hooks.h"
#include "core/Util.h"
#include "core/Offsets.h"

#pragma comment(lib, "Ws2_32.lib")

using DirectInput8Create_t = HRESULT(WINAPI*)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
DirectInput8Create_t g_originalDirectInput8Create = nullptr;

extern "C" __declspec(dllexport) HRESULT WINAPI DirectInput8Create(
    HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
    if (g_originalDirectInput8Create)
    {
        return g_originalDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
    }

    return HRESULT_FROM_WIN32(ERROR_PROC_NOT_FOUND);
}

inline bool CheckGameId(uintptr_t appIdPtr, std::string_view prefix)
{
    return appIdPtr && std::string_view(reinterpret_cast<const char*>(appIdPtr)).starts_with(prefix);
}

void wsaStartup()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0)
    {
        GG_LOG(GG::LogLevel::Error, "WSAStartup failed: %d\n", result);
        return;
    }

    GG_LOG(GG::LogLevel::Info, "Winsock ready (version %d.%d)\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
}

void Initialize()
{
    g_game = std::make_unique<GG::Game>();
    g_program = std::make_unique<Program>();
    g_program->initialize();

    auto& hm = GG::HookManager::getManager();

    hm.initialize();

    if (CheckGameId(offsets::gw1::g_AppId, "PvZ Garden Warfare"))
    {
        GG_LOG(GG::LogLevel::Info, "Identified: PvZ Garden Warfare");

        g_game->initialize(GG::GameVersion::GW1);
        hm.registerHooks(g_PvZGW1_Hooks);

        ApplyPatch(offsets::gw1::patch_BackendOnMessage);
        ApplyPatch(offsets::gw1::patch_AllowCommandArgumentsAsOptions);
        ApplyPatch(offsets::gw1::patch_AllowMultipleInstances);
        ApplyPatch(offsets::gw1::patch_DisableTutorials);
        ApplyPatch(offsets::gw1::patch_FixAvailableConsumables);
        ApplyPatch(offsets::gw1::patch_FixHolidayConsumables);
    }
    else if (CheckGameId(offsets::gw2::g_AppId, "PvZGW2"))
    {
        GG_LOG(GG::LogLevel::Info, "Identified: PvZ Garden Warfare 2");

        g_game->initialize(GG::GameVersion::GW2);
        hm.registerHooks(g_PvZGW2_Hooks);

        ApplyPatch(offsets::gw2::patch_AllowCommandArgumentsAsOptions);
        ApplyPatch(offsets::gw2::patch_AllowMultipleInstances);
        ApplyPatch(offsets::gw2::patch_PlayerNameFormatting);
        ApplyPatch(offsets::gw2::patch_SkipStreamInstallInstallation);
        ApplyPatch(offsets::gw2::patch_MemFreeBug);
        ApplyPatch(offsets::gw2::patch_InfiniteConsumables);
    }
    else if (CheckGameId(offsets::gw3::g_AppId, "PvZGW3"))
    {
        GG_LOG(GG::LogLevel::Info, "Identified: PvZ Garden Warfare 3");
        GG_LOG(GG::LogLevel::Info, "Identified: ....lol...just kidding...");
        GG_LOG(GG::LogLevel::Info, "Identified: PvZ Battle for Neighborville");

        g_game->initialize(GG::GameVersion::GW3);
        hm.registerHooks(g_PvZGW3_Hooks);

        ApplyPatch(offsets::gw3::patch_AllowCommandArgumentsAsOptions);
        ApplyPatch(offsets::gw3::patch_IgnoreEasyAntiCheat);
        ApplyPatch(offsets::gw3::patch_Killswitches);
        ApplyPatch(offsets::gw3::patch_OfflineFix);
        ApplyPatch(offsets::gw3::patch_UnlockAwards);

        ApplyPatch(offsets::gw3::patch_MemFreeBug);
        ApplyPatch(offsets::gw3::patch_MultipleInstances);
    }
    else
    {
        GG_FATAL("Could not identify game!");
    }
}

DWORD WINAPI ggThread(LPVOID hInstance)
{
    g_program->run();
    g_program->uninitialize();

    g_game.reset();
    g_program.reset();

    WSACleanup();

    FreeLibraryAndExitThread(reinterpret_cast<HINSTANCE>(hInstance), 0);
}

bool hookDinput8()
{
    std::wstring systemPath(MAX_PATH, L'\0');
    UINT pathLen = GetSystemDirectoryW(systemPath.data(), MAX_PATH);
    systemPath.resize(pathLen);
    systemPath += L"\\dinput8.dll";

    HMODULE original = LoadLibraryW(systemPath.c_str());
    if (!original)
        return false;

    FARPROC proc = GetProcAddress(original, "DirectInput8Create");
    if (!proc)
        return false;

    g_originalDirectInput8Create = reinterpret_cast<DirectInput8Create_t>(proc);
    return g_originalDirectInput8Create != nullptr;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved [[maybe_unused]])
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hinstDLL);

        if (!hookDinput8())
            std::cerr << "Failed to hook dinput8.dll" << std::endl;

        Initialize();
        wsaStartup();

        HANDLE threadHandle = CreateThread(nullptr, 0, ggThread, hinstDLL, 0, nullptr);
        if (threadHandle)
            CloseHandle(threadHandle);
        break;
    }

    case DLL_PROCESS_DETACH:
        if (g_program)
        {
            g_program->stop();
        }
        break;
    }

    return TRUE;
}
