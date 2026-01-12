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

using DirectInput8Create_t = HRESULT(WINAPI *)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
DirectInput8Create_t g_originalDirectInput8Create = nullptr;

extern "C" __declspec(dllexport) HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
    if (g_originalDirectInput8Create) {
        return g_originalDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
    }

    return HRESULT_FROM_WIN32(ERROR_PROC_NOT_FOUND);
}

void Initialize() {
    auto& hm = GG::HookManager::getManager();
    hm.initialize();

    std::string_view gw1_appId(reinterpret_cast<const char*>(offsets::gw1::g_AppId));
    std::string_view gw2_appId(reinterpret_cast<const char*>(offsets::gw2::g_AppId));

    if (gw1_appId.starts_with("PvZ Garden Warfare"))
    {
        GG_LOG(GG::LogLevel::Info, "Identified: PvZ Garden Warfare");

        hm.registerHooks(g_PvZGW1_Hooks);

        ApplyPatch(offsets::gw1::patch_AllowCommandArgumentsAsOptions);
        ApplyPatch(offsets::gw1::patch_FixLevelReportInstanceIndex);
        ApplyPatch(offsets::gw1::patch_FixAvailableConsumables);
    }
    else if (gw2_appId.starts_with("PvZGW2"))
    {
        GG_LOG(GG::LogLevel::Info, "Identified: PvZ Garden Warfare 2");

        hm.registerHooks(g_PvZGW2_Hooks);

        ApplyPatch(offsets::gw2::patch_AllowCommandArgumentsAsOptions);
        ApplyPatch(offsets::gw2::patch_PlayerNameFormatting);
    }
    else {
        GG_FATAL("Could not identify game!");
    }
}

DWORD WINAPI ggThread(LPVOID hInstance) {
    g_game = std::make_unique<GG::Game>();
    g_game->initialize();

    g_program = std::make_unique<Program>();
    g_program->initialize();

    Initialize();

    g_program->run();
    g_program->uninitialize();

    g_game.reset();
    g_program.reset();

    FreeLibraryAndExitThread(reinterpret_cast<HINSTANCE>(hInstance), 0);
}

bool hookDinput8() {
    std::wstring systemPath(MAX_PATH, L'\0');
    UINT pathLen = GetSystemDirectoryW(systemPath.data(), MAX_PATH);
    systemPath.resize(pathLen);
    systemPath += L"\\dinput8.dll";

    HMODULE original = LoadLibraryW(systemPath.c_str());
    if (!original) return false;

    FARPROC proc = GetProcAddress(original, "DirectInput8Create");
    if (!proc) return false;

    g_originalDirectInput8Create = reinterpret_cast<DirectInput8Create_t>(proc);
    return g_originalDirectInput8Create != nullptr;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved [[maybe_unused]] )
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hinstDLL);

        if (!hookDinput8())
            std::cerr << "Failed to hook dinput8.dll" << std::endl;

        HANDLE threadHandle = CreateThread(nullptr, 0, ggThread, hinstDLL, 0, nullptr);
        if (threadHandle) CloseHandle(threadHandle);
        break;
    }

    case DLL_PROCESS_DETACH:
        if (g_program) {
            g_program->stop();
        }
        break;
    }

    return TRUE;
}
