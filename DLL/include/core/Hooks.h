#pragma once

#include "base/Log.h"
#include "core/HookManager.h"
#include "core/Game.h"
#include "core/Offsets.h"
#include "sdk/Types.h"
#include "sdk/Main.h"
#include "sdk/ServerSpawnInfo.h"
#include "sdk/ServerSpawnOverrides.h"
#include "sdk/SocketManagerFactory.h"
#include "sdk/Messages.h"
#include "sdk/SecureReason.h"
#include "sdk/Settings.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <mutex>
#include <cstdint>

namespace fb
{
// ============================================================================
// PvZ: Garden Warfare 1
// ============================================================================
namespace gw1
{
using tGetOptionParameter = char* (*)(const char*, const char*, int*);
inline tGetOptionParameter GetOptionParameter = reinterpret_cast<tGetOptionParameter>(offsets::gw1::fn_GetOptionParameter);

using tSetInclusionOptions = intptr_t(__fastcall*)(LevelSetup*, const char*);
inline tSetInclusionOptions setInclusionOptions = reinterpret_cast<tSetInclusionOptions>(offsets::gw1::fn_SetInclusionOptions);

using tCreateServer = intptr_t(__fastcall*)(intptr_t, ServerSpawnInfo*);
inline tCreateServer CreateServer = reinterpret_cast<tCreateServer>(offsets::gw1::fn_CreateServer);

using tLevelSetupInit = intptr_t(__fastcall*)(LevelSetup*);
inline tLevelSetupInit LevelSetupInit = reinterpret_cast<tLevelSetupInit>(offsets::gw1::fn_LevelSetupInit);

template<typename T>
T* getContainer(const char* identifier)
{
    using GetContainerFn = __int64(__fastcall*)(__int64, __int64);
    auto fn = reinterpret_cast<GetContainerFn>(offsets::gw1::fn_GetContainer);
    __int64 settingsMgr = *reinterpret_cast<__int64*>(offsets::gw1::g_SettingsManager);
    __int64 container = fn(settingsMgr, reinterpret_cast<__int64>(identifier));
    return reinterpret_cast<T*>(container);
}

intptr_t initSettings(intptr_t inst)
{
    const auto trampoline = GG::HookManager::getManager().Call(&initSettings);
    auto ret = trampoline(inst);

    GG_LOG(GG::LogLevel::Debug, "fb::Main::initSettings(this: %p)", inst);

    auto onlineSettings = getContainer<OnlineSettings>("Online");
    auto serverSettings = getContainer<ServerSettings>("Server");
    auto syncedSettings = getContainer<SyncedBFSettings>("SyncedBFSettings");

    onlineSettings->Backend = Backend_Peer;
    onlineSettings->ServerAllowAnyReputation = true;
    serverSettings->IsRanked = false;
    syncedSettings->AllUnlocksUnlocked = true;

    return ret;
}

void initDedicatedServer(fb::Main* inst, intptr_t platformHook)
{
    GG_LOG(GG::LogLevel::Debug, "fb::Main::initDedicatedServer(this: %p, platformHook: %p)", inst, platformHook);

    LevelSetup setup{ 0 };

    LevelSetupInit(&setup);

    setup.Name = GetOptionParameter("level", "_pvz/Levels/Coastal/Level_COOP_Coastal/Level_COOP_Coastal", 0);
    setInclusionOptions(&setup, GetOptionParameter("Game.DefaultLayerInclusion", "GameMode=Coop0;TOD=Day", 0));

    ServerSpawnInfo spawnInfo(setup);
    spawnInfo.tickFrequency = 30;
    spawnInfo.isEncrypted = false;
    spawnInfo.isSinglePlayer = false;
    spawnInfo.isLocalHost = false;
    spawnInfo.isDedicated = true;
    spawnInfo.isCoop = false;
    spawnInfo.isMenu = false;
    spawnInfo.keepResources = false;

    auto server = CreateServer(reinterpret_cast<intptr_t>(inst), &spawnInfo);

    if (server)
    {
        GG_LOG(GG::LogLevel::Info, "[SERVER] Created server instance.");
    }
    else
    {
        GG_LOG(GG::LogLevel::Error, "[SERVER] Failed to create server instance.");
    }
}

intptr_t MainInit(fb::Main* inst, void* platformHook)
{
    const auto trampoline = GG::HookManager::getManager().Call(&MainInit);

    GG_LOG(GG::LogLevel::Debug, "fb::Main::init(this: %p, platformHook: %p)", inst, platformHook);

    GG::HookManager::HookVTableFunction(reinterpret_cast<void*>(inst), &initDedicatedServer, 52);

    inst->m_isDedicatedServer = GetOptionParameter("server", nullptr, 0) != nullptr;

    return trampoline(inst, platformHook);
};

__int64 ServerStart(intptr_t inst, ServerSpawnInfo& info, ServerSpawnOverrides* spawnOverrides)
{
    const auto trampoline = GG::HookManager::getManager().Call(&ServerStart);

    if (!info.isLocalHost)
    {
        spawnOverrides->socketManager = g_game->getSocketManager();
        g_game->setHosting(true);
    }
    else
    {
        g_game->setHosting(false);
    }

    GG_LOG(GG::LogLevel::Debug, "ServerStart - inst:%p, info:%p, spawnOverrides:%p, socketManager:%p", (intptr_t*)inst, (intptr_t*)&info,
        (intptr_t*)spawnOverrides, (intptr_t*)spawnOverrides->socketManager);

    GG_LOG(GG::LogLevel::Debug, "info->levelSetup->Name = %s", info.levelSetup.Name);

    for (int i = 0; i < (int)info.levelSetup.InclusionOptions.size(); ++i)
    {
        const auto& opt = info.levelSetup.InclusionOptions[i];
        const char* key = opt.m_criterion ? opt.m_criterion : "<null>";
        const char* value = opt.m_value ? opt.m_value : "<null>";

        GG_LOG(GG::LogLevel::Debug, "info->levelSetup->m_inclusionOptions[k:v] = %s:%s", key, value);
    }

    GG_LOG(GG::LogLevel::Debug, "info->levelSetup->StartPoint = %s", info.levelSetup.StartPoint);
    GG_LOG(GG::LogLevel::Debug, "info->tickFrequency = %d", info.tickFrequency);
    GG_LOG(GG::LogLevel::Debug, "info->isSinglePlayer = %d", (int)info.isSinglePlayer);
    GG_LOG(GG::LogLevel::Debug, "info->isLocalHost = %d", (int)info.isLocalHost);
    GG_LOG(GG::LogLevel::Debug, "info->isDedicated = %d", (int)info.isDedicated);
    GG_LOG(GG::LogLevel::Debug, "info->isEncrypted = %d", (int)info.isEncrypted);
    GG_LOG(GG::LogLevel::Debug, "info->isCoop = %d", (int)info.isCoop);
    GG_LOG(GG::LogLevel::Debug, "info->isMenu = %d", (int)info.isMenu);
    GG_LOG(GG::LogLevel::Debug, "info->keepResources = %d", (int)info.keepResources);

    return trampoline(inst, info, spawnOverrides);
}

bool ClientInitNetwork(intptr_t inst, bool singleplayer, bool localhost, bool coop, bool hosted)
{
    const auto trampoline = GG::HookManager::getManager().Call(&ClientInitNetwork);

    GG_LOG(GG::LogLevel::Info, "fb::client::InitNetwork(singleplayer: %d, localhost: %d, coop: %d, hosted: %d)", (char)singleplayer,
        (char)localhost, (char)coop, (char)hosted);

    if (hosted || !singleplayer)
    {
        g_game->injectSocketManagerFactory(inst, 0xA8);

        singleplayer = false;
        localhost = false;
        coop = false;
        hosted = false;
    }

    return trampoline(inst, singleplayer, localhost, coop, hosted);
}

void ClientConnectToAddress(intptr_t inst, const char* ipAddress, const char* serverPassword)
{
    const auto trampoline = GG::HookManager::getManager().Call(&ClientConnectToAddress);

    ipAddress = g_game->redirectHostingAddress(ipAddress);
    serverPassword = GetOptionParameter("Server.ServerPassword", "", 0);
    g_game->logClientConnect(ipAddress, serverPassword);

    return trampoline(inst, ipAddress, serverPassword);
}

char NetworkEnginePeerInit(intptr_t inst, __int64 crypto, intptr_t socketManager, const char* address, int titleId, int versionId,
    bool debug, bool voice, bool live)
{
    const auto trampoline = GG::HookManager::getManager().Call(&NetworkEnginePeerInit);

    GG_LOG(GG::LogLevel::Debug, "NetworkEnginePeerInit - inst:%p, socketManager:%p, address: %s, titleId: %d, versionId: %d",
        (intptr_t*)inst, (intptr_t*)socketManager, address, titleId, versionId);

    std::string_view addressView{ address };
    if (const auto colon = addressView.find(':'); colon != std::string_view::npos)
    {
        const auto port = addressView.substr(colon + 1);
        address = g_game->adjustPeerAddress(address, port);
    }

    return trampoline(inst, crypto, socketManager, address, titleId, versionId, debug, voice, live);
}

void __fastcall ClientInactivityTimer(intptr_t, float)
{
    return;
}

char* GetPlayerName()
{
    return GetOptionParameter("name", "Player", 0);
}

void MessageManagerDispatchMessage(void* inst, Message* message)
{
    const auto trampoline = GG::HookManager::getManager().Call(&MessageManagerDispatchMessage);

    if (!message)
        return trampoline(inst, message);

    auto* type = message->getType();
    if (!type || !type->typeInfoData)
        return trampoline(inst, message);

    const char* name = type->getName();
    if (!name)
        return trampoline(inst, message);

    GG_LOG(GG::LogLevel::DebugPlusPlus, "[Message] %s, type: %d, category: %d", name, message->type, message->category);

    if (name != nullptr && std::strcmp(name, "ServerBackendBattlepackRequestMessageBase") == 0)
    {
        return;
    }

    if (name != nullptr && std::strcmp(name, "ServerLevelLoadedMessage") == 0)
    {
        GG_LOG(GG::LogLevel::Info, "[SERVER] Level Loaded.");
    }

    if (name != nullptr && std::strcmp(name, "ServerLevelCompletedMessage") == 0)
    {
        GG_LOG(GG::LogLevel::Info, "[SERVER] Level completed.");
    }

    if (name != nullptr && std::strcmp(name, "ServerPlayerCreateMessage") == 0)
    {
        auto msg = reinterpret_cast<ServerPlayerCreateMessage*>(message);

        if (msg->m_player != nullptr)
        {
            GG_LOG(GG::LogLevel::Info, "[SERVER] %s has joined.", msg->m_player->m_name);
        }
        else
        {
            GG_LOG(GG::LogLevel::Info, "[SERVER] Someone has joined.");
        }
    }

    if (name != nullptr && std::strcmp(name, "ServerPlayerDestroyMessage") == 0)
    {
        auto msg = reinterpret_cast<ServerPlayerDestroyMessage*>(message);

        if (msg->m_player != nullptr)
        {
            GG_LOG(GG::LogLevel::Info, "[SERVER] %s has left.", msg->m_player->m_name);
        }
        else
        {
            GG_LOG(GG::LogLevel::Info, "[SERVER] Someone has left.");
        }
    }

    if (name != nullptr && std::strcmp(name, "NetworkDisconnectedMessage") == 0)
    {
        auto msg = reinterpret_cast<NetworkDisconnectedMessage*>(message);

        g_game->handleDisconnection(msg->m_reason);
    }

    trampoline(inst, message);
}
} // namespace gw1

// ============================================================================
// PvZ: Garden Warfare 2
// ============================================================================
namespace gw2
{
using tGetOptionParameter = char* (*)(const char*, const char*, int*);
inline tGetOptionParameter GetOptionParameter = reinterpret_cast<tGetOptionParameter>(offsets::gw2::fn_GetOptionParameter);

using tSetInclusionOptions = intptr_t(__fastcall*)(LevelSetup*, const char*);
inline tSetInclusionOptions setInclusionOptions = reinterpret_cast<tSetInclusionOptions>(offsets::gw2::fn_SetInclusionOptions);

using tCreateServer = intptr_t(__fastcall*)(intptr_t, ServerSpawnInfo*);
inline tCreateServer CreateServer = reinterpret_cast<tCreateServer>(offsets::gw2::fn_CreateServer);

template<typename T>
T* getContainer(const char* identifier)
{
    using GetContainerFn = __int64(__fastcall*)(__int64, __int64);
    auto fn = reinterpret_cast<GetContainerFn>(offsets::gw2::fn_GetContainer);
    __int64 settingsMgr = *reinterpret_cast<__int64*>(offsets::gw2::g_SettingsManager);
    __int64 container = fn(settingsMgr, reinterpret_cast<__int64>(identifier));
    return reinterpret_cast<T*>(container);
}

intptr_t initSettings(intptr_t inst)
{
    const auto trampoline = GG::HookManager::getManager().Call(&initSettings);
    auto ret = trampoline(inst);

    GG_LOG(GG::LogLevel::Debug, "fb::Main::initSettings(this: %p)", inst);

    auto onlineSettings = getContainer<PVZOnlineSettings>("Online");
    auto gameSettings = getContainer<GameSettings>("Game");
    auto gameModeSettings = getContainer<GameModeSettings>("GameMode");
    auto pvzServerSettings = getContainer<PVZServerSettings>("PVZServer");
    auto networkSettings = getContainer<NetworkSettings>("Network");

    gameSettings->Platform = GamePlatform_Win32;
    onlineSettings->Backend = Backend_Local;
    onlineSettings->PeerBackend = Backend_LocalNoStorage;
    onlineSettings->ClientIsPresenceEnabled = true;
    onlineSettings->ServerIsPresenceEnabled = true;
    onlineSettings->OnlineGameInteractionMasterKillSwitch = true;
    onlineSettings->OnlineGameInteractionKillSwitchList = reinterpret_cast<char*>(offsets::gw2::g_EmptyArray);
    networkSettings->MaxClientCount = 64;
    pvzServerSettings->KickIdlePlayers = false;
    gameModeSettings->SkipPreroundCountdown = true;
    gameModeSettings->SkipIntroHubNIS = true;

    return ret;
}

void ServerLoadLevelMessagePost(LevelSetup* levelSetup, bool fadeOut, bool forceReloadResources)
{
    const auto trampoline = GG::HookManager::getManager().Call(&ServerLoadLevelMessagePost);

    GG_LOG(GG::LogLevel::Debug, "[SERVER] Loading level: %s", levelSetup->Name);

    return trampoline(levelSetup, fadeOut, forceReloadResources);
}

char initDedicatedServer(fb::Main* inst, intptr_t platformHook)
{
    GG_LOG(GG::LogLevel::Debug, "fb::Main::initDedicatedServer(this: %p, platformHook: %p)", inst, platformHook);

    LevelSetup setup{ 0 };
    setup.InclusionOptions.set(reinterpret_cast<LevelSetupOption*>(offsets::gw2::g_EmptyArray));
    setup.Name = GetOptionParameter("level", "Levels/Level_FE_Hub/Level_FE_Hub", 0);
    setup.StartPoint = reinterpret_cast<char*>(offsets::gw2::g_EmptyStr);
    setup.DifficultyIndex = 0;
    setup.ForceReloadResources = false;
    setup.IsSaveGame = false;
    setup.Playlist = reinterpret_cast<char*>(offsets::gw2::g_EmptyStr);
    setup.LoadScreen_GameMode = reinterpret_cast<char*>(offsets::gw2::g_EmptyStr);
    setup.LoadScreen_LevelDescription = reinterpret_cast<char*>(offsets::gw2::g_EmptyStr);
    setup.LoadScreen_LevelAsset = reinterpret_cast<char*>(offsets::gw2::g_EmptyStr);
    setup.LoadScreen_LevelName = reinterpret_cast<char*>(offsets::gw2::g_EmptyStr);

    setInclusionOptions(&setup, GetOptionParameter("Game.DefaultLayerInclusion", "GameMode=FreeRoam;TOD=Day", 0));

    ServerSpawnInfo spawnInfo(setup);
    spawnInfo.tickFrequency = 30;
    spawnInfo.isSinglePlayer = false;
    spawnInfo.isLocalHost = false;
    spawnInfo.isDedicated = true;
    spawnInfo.isCoop = false;
    spawnInfo.isMenu = false;
    spawnInfo.keepResources = true;
    spawnInfo.validLocalPlayersMask = 0x1;
    spawnInfo.saveData.init(0);

    auto server = CreateServer(reinterpret_cast<intptr_t>(inst), &spawnInfo);

    if (server)
    {
        GG_LOG(GG::LogLevel::Debug, "Created server instance.");
    }
    else
    {
        GG_LOG(GG::LogLevel::Error, "Failed to create server instance.");
    }

    return 1;
}

intptr_t MainInit(fb::Main* inst, void* platformHook)
{
    const auto trampoline = GG::HookManager::getManager().Call(&MainInit);

    GG_LOG(GG::LogLevel::Debug, "fb::Main::init(this: %p, platformHook: %p)", inst, platformHook);

    GG::HookManager::HookVTableFunction(reinterpret_cast<void*>(inst), &initDedicatedServer, 57);

    inst->m_isDedicatedServer = GetOptionParameter("server", nullptr, 0) != nullptr;

    return trampoline(inst, platformHook);
};

intptr_t ServerStart(intptr_t inst, ServerSpawnInfo& info, ServerSpawnOverrides* spawnOverrides)
{
    const auto trampoline = GG::HookManager::getManager().Call(&ServerStart);

    spawnOverrides->socketManager = g_game->getSocketManager();

    g_game->setHosting(true);

    GG_LOG(GG::LogLevel::Debug, "ServerStart - inst:%p, info:%p, spawnOverrides:%p, socketManager:%p", (intptr_t*)inst, (intptr_t*)&info,
        (intptr_t*)spawnOverrides, (intptr_t*)spawnOverrides->socketManager);

    GG_LOG(GG::LogLevel::Debug, "info->levelSetup->Name = %s", info.levelSetup.Name);

    for (int i = 0; i < (int)info.levelSetup.InclusionOptions.size(); ++i)
    {
        const auto& opt = info.levelSetup.InclusionOptions[i];
        const char* key = opt.m_criterion ? opt.m_criterion : "<null>";
        const char* value = opt.m_value ? opt.m_value : "<null>";

        GG_LOG(GG::LogLevel::Debug, "info->levelSetup->m_inclusionOptions[k:v] = %s:%s", key, value);
    }

    GG_LOG(GG::LogLevel::Debug, "info->levelSetup->StartPoint = %s", info.levelSetup.StartPoint);
    GG_LOG(GG::LogLevel::Debug, "info->tickFrequency = %d", info.tickFrequency);
    GG_LOG(GG::LogLevel::Debug, "info->isSinglePlayer = %d", (int)info.isSinglePlayer);
    GG_LOG(GG::LogLevel::Debug, "info->isLocalHost = %d", (int)info.isLocalHost);
    GG_LOG(GG::LogLevel::Debug, "info->isDedicated = %d", (int)info.isDedicated);
    GG_LOG(GG::LogLevel::Debug, "info->isEncrypted = %d", (int)info.isEncrypted);
    GG_LOG(GG::LogLevel::Debug, "info->isCoop = %d", (int)info.isCoop);
    GG_LOG(GG::LogLevel::Debug, "info->isMenu = %d", (int)info.isMenu);
    GG_LOG(GG::LogLevel::Debug, "info->keepResources = %d", (int)info.keepResources);

    return trampoline(inst, info, spawnOverrides);
}

bool ClientInitNetwork(intptr_t inst, char idk, bool localhost, bool coop, bool hosted, bool singleplayer)
{
    const auto trampoline = GG::HookManager::getManager().Call(&ClientInitNetwork);

    GG_LOG(GG::LogLevel::Info, "fb::client::InitNetwork(idk: %d, singleplayer: %d, localhost: %d, coop: %d, hosted: %d)", (char)idk,
        (char)singleplayer, (char)localhost, (char)coop, (char)hosted);

    g_game->injectSocketManagerFactory(inst, 0xB8);

    localhost = false;
    singleplayer = false;
    coop = false;
    hosted = true;

    return trampoline(inst, idk, localhost, coop, hosted, singleplayer);
}

void ClientConnectToAddress(intptr_t inst, const char* ipAddress, const char* serverPassword)
{
    const auto trampoline = GG::HookManager::getManager().Call(&ClientConnectToAddress);

    ipAddress = g_game->redirectHostingAddress(ipAddress);
    serverPassword = GetOptionParameter("Server.ServerPassword", "", 0);
    g_game->logClientConnect(ipAddress, serverPassword);

    return trampoline(inst, ipAddress, serverPassword);
}

char NetworkEnginePeerInit(intptr_t inst, __int64 crypto, intptr_t socketManager, const char* address, int titleId, int versionId)
{
    const auto trampoline = GG::HookManager::getManager().Call(&NetworkEnginePeerInit);

    GG_LOG(GG::LogLevel::Debug, "NetworkEnginePeerInit - inst:%p, socketManager:%p, address: %s, titleId: %d, versionId: %d",
        (intptr_t*)inst, (intptr_t*)socketManager, address, titleId, versionId);

    std::string_view addressView{ address };
    if (const auto colon = addressView.find(':'); colon != std::string_view::npos)
    {
        const auto port = addressView.substr(colon + 1);
        address = g_game->adjustPeerAddress(address, port);
    }

    return trampoline(inst, crypto, socketManager, address, titleId, versionId);
}

char* GetPlayerName()
{
    return GetOptionParameter("name", "Player", 0);
}

void MessageManagerExecuteMessage(void* inst, Message* message)
{
    const auto trampoline = GG::HookManager::getManager().Call(&MessageManagerExecuteMessage);

    if (!message)
        return trampoline(inst, message);

    auto* type = message->getType();
    if (!type || !type->typeInfoData)
        return trampoline(inst, message);

    const char* name = type->getName();
    if (!name)
        return trampoline(inst, message);

    GG_LOG(GG::LogLevel::DebugPlusPlus, "[Message] %s, type: %d, category: %d, playerId: %d", name, message->type, message->category,
        message->LocalPlayerId);

    if (name != nullptr && std::strcmp(name, "ServerLevelLoadedMessage") == 0)
    {
        GG_LOG(GG::LogLevel::Info, "[SERVER] Level Loaded.");
    }

    if (name != nullptr && std::strcmp(name, "NetworkCreatePlayerMessage") == 0)
    {
        auto msg = reinterpret_cast<NetworkCreatePlayerMessage*>(message);

        if (msg->m_name != nullptr)
        {
            GG_LOG(GG::LogLevel::Info, "[SERVER] %s has joined.", msg->m_name);
        }
        else
        {
            GG_LOG(GG::LogLevel::Info, "[SERVER] Someone has joined.");
        }
    }

    if (name != nullptr && std::strcmp(name, "ServerPlayerDestroyMessage") == 0)
    {
        auto msg = reinterpret_cast<ServerPlayerDestroyMessage*>(message);

        if (msg->m_player != nullptr)
        {
            GG_LOG(GG::LogLevel::Info, "[SERVER] %s has left.", msg->m_player->m_name);
        }
        else
        {
            GG_LOG(GG::LogLevel::Info, "[SERVER] Someone has left.");
        }
    }

    if (name != nullptr && std::strcmp(name, "NetworkDisconnectedMessage") == 0)
    {
        auto msg = reinterpret_cast<NetworkDisconnectedMessage*>(message);

        g_game->handleDisconnection(msg->m_reason);
    }

    trampoline(inst, message);
}

int getPlatformDataType(char*)
{
    return 1;
}

int getBackendType(ServerSpawnInfo*)
{
    return Backend_Local;
}
} // namespace gw2

// ============================================================================
// PvZ:Battle for Neighborville
// ============================================================================
namespace gw3
{
static bool splash = false;

using tGetOptionParameter = char* (*)(const char*, const char*, int*);
inline tGetOptionParameter GetOptionParameter = reinterpret_cast<tGetOptionParameter>(offsets::gw3::fn_GetOptionParameter);

using tGetSettings = intptr_t (*)(intptr_t, intptr_t);
template<typename T>
inline T* GetSettings(intptr_t settingsManager, intptr_t settingsInstance)
{
    auto fn = reinterpret_cast<tGetSettings>(offsets::gw3::fn_GetSettings);

    intptr_t mgr = *reinterpret_cast<intptr_t*>(settingsManager);
    intptr_t ret = fn(mgr, settingsInstance);
    return reinterpret_cast<T*>(ret);
}

using tLevelSetupInit = intptr_t(__fastcall*)(LevelSetup*);
inline tLevelSetupInit LevelSetupInit = reinterpret_cast<tLevelSetupInit>(offsets::gw3::fn_LevelSetupInit);

using tSetInclusionOptions = intptr_t(__fastcall*)(LevelSetup*, const char*);
inline tSetInclusionOptions SetInclusionOptions = reinterpret_cast<tSetInclusionOptions>(offsets::gw3::fn_SetInclusionOptions);

using tfbString = void (*)(intptr_t, const char*, unsigned int);
inline tfbString fbString = reinterpret_cast<tfbString>(offsets::gw3::fn_fbString);

using tUserAdded = void (*)(void*, void**, unsigned int);
inline tUserAdded UserAdded = reinterpret_cast<tUserAdded>(offsets::gw3::fn_UserAdded);

using tSpawnServer = intptr_t(__fastcall*)(intptr_t, ServerSpawnInfo*);
inline tSpawnServer SpawnServer = reinterpret_cast<tSpawnServer>(offsets::gw3::fn_SpawnServer);

void initSettings()
{
    auto netObjectSettings = GetSettings<NetObjectSystemSettings>(offsets::gw3::g_SettingsManager, offsets::gw3::g_NetObjectSettings);
    netObjectSettings->MaxServerConnectionCount = 64;

    auto serverSettings = GetSettings<PVZServerSettings>(offsets::gw3::g_SettingsManager, offsets::gw3::g_PVZServerSettings);
    serverSettings->KickIdlePlayers = false;

    auto pvzOnlineSettings = GetSettings<PVZOnlineSettings>(offsets::gw3::g_SettingsManager, offsets::gw3::g_PVZOnlineSettings);
    pvzOnlineSettings->ShouldControlBlaze = false;
    pvzOnlineSettings->ClientIsPresenceEnabled = false;
    pvzOnlineSettings->ServerIsPresenceEnabled = false;
    pvzOnlineSettings->ServerAllowAnyReputation = true;

    auto gameModeSettings = GetSettings<GameModeSettings>(offsets::gw3::g_SettingsManager, offsets::gw3::g_GameModeSettings);
    gameModeSettings->ShouldSkipHUBTutorial = true;
    gameModeSettings->SocialHUBSkipStationTutorials = true;
    gameModeSettings->SocialHUBSkipLandingPage = true;
    auto hubSeason = GetOptionParameter("GameModeSettings.ForceHUBSeason", "", 0);
    if (hubSeason[0] != '\0')
        gameModeSettings->ForceHUBSeason = atoi(hubSeason);
}

intptr_t ServerStart(intptr_t inst, ServerSpawnInfo& info, ServerSpawnOverrides* spawnOverrides, fb::ISocketManager* socketManager)
{
    const auto trampoline = GG::HookManager::getManager().Call(&ServerStart);

    initSettings();

    GG_LOG(GG::LogLevel::Debug, "fb::Server::start(inst:%p, info:%p, spawnOverrides:%p, socketManager:%p)", (intptr_t*)inst,
        (intptr_t*)&info, (intptr_t*)spawnOverrides, (intptr_t*)socketManager);

    fb::gw3::LevelSetup* levelSetup = reinterpret_cast<fb::gw3::LevelSetup*>(&info.levelSetup);

    GG_LOG(GG::LogLevel::Debug, "info->levelSetup->Name = %s", levelSetup->Name);
    GG_LOG(GG::LogLevel::Debug, "info->levelSetup->StartPoint = %s", levelSetup->StartPoint);
    GG_LOG(GG::LogLevel::Debug, "info->levelSetup->LevelManagerInitialLevel = %s", levelSetup->LevelManagerInitialLevel);
    GG_LOG(GG::LogLevel::Debug, "info->levelSetup->LevelManagerStartPoint = %s", levelSetup->LevelManagerStartPoint);

    for (int i = 0; i < (int)levelSetup->InclusionOptions.size(); ++i)
    {
        const auto& opt = levelSetup->InclusionOptions[i];
        const char* key = opt.m_criterion ? opt.m_criterion : "<null>";
        const char* value = opt.m_value ? opt.m_value : "<null>";

        GG_LOG(GG::LogLevel::Debug, "info->levelSetup->m_inclusionOptions[k:v] = %s:%s", key, value);
    }

    GG_LOG(GG::LogLevel::Debug, "info->tickFrequency = %d", info.tickFrequency);
    GG_LOG(GG::LogLevel::Debug, "info->isSinglePlayer = %d", (int)info.isSinglePlayer);
    GG_LOG(GG::LogLevel::Debug, "info->isLocalHost = %d", (int)info.isLocalHost);
    GG_LOG(GG::LogLevel::Debug, "info->isDedicated = %d", (int)info.isDedicated);
    GG_LOG(GG::LogLevel::Debug, "info->isEncrypted = %d", (int)info.isEncrypted);
    GG_LOG(GG::LogLevel::Debug, "info->isCoop = %d", (int)info.isCoop);
    GG_LOG(GG::LogLevel::Debug, "info->isMenu = %d", (int)info.isMenu);
    GG_LOG(GG::LogLevel::Debug, "info->keepResources = %d", (int)info.keepResources);

    std::string_view levelName(levelSetup->Name);

    if (!levelName.starts_with("Levels/Level_Picnic_Splash/Level_Picnic_Splash"))
    {
        GG_LOG(GG::LogLevel::Debug, "Forcing non-localhost");
        g_game->setHosting(true);

        info.isLocalHost = false;

        spawnOverrides->socketManager = g_game->getSocketManager();
    }
    else
    {
        splash = true;
    }

    return trampoline(inst, info, spawnOverrides, socketManager);
}

bool ClientInitNetwork(intptr_t inst, bool singleplayer, bool localhost, bool coop, bool hosted)
{
    const auto trampoline = GG::HookManager::getManager().Call(&ClientInitNetwork);

    // g_game->logClientInitNetwork(singleplayer, localhost, coop, hosted);

    if (splash)
    {
        g_game->injectSocketManagerFactory(inst, 0x168);

        GG_LOG(GG::LogLevel::Debug, "Injected socketmanager.");
    }

    return trampoline(inst, singleplayer, localhost, coop, hosted);
}

void ClientConnectToAddress(intptr_t inst, const char* ipAddress, const char* serverPassword)
{
    const auto trampoline = GG::HookManager::getManager().Call(&ClientConnectToAddress);

    ipAddress = g_game->redirectHostingAddress(ipAddress);
    serverPassword = GetOptionParameter("Server.ServerPassword", "", 0);

    if (g_game->isJoining())
    {
        ipAddress = GetOptionParameter("Client.ServerIp", "", 0);
    }

    g_game->logClientConnect(ipAddress, serverPassword);

    return trampoline(inst, ipAddress, serverPassword);
}

char NetworkEnginePeerInit(intptr_t inst, intptr_t socketManager, const char* address, int titleId, int versionId)
{
    const auto trampoline = GG::HookManager::getManager().Call(&NetworkEnginePeerInit);

    GG_LOG(GG::LogLevel::Debug, "NetworkEnginePeerInit - inst:%p, socketManager:%p, address: %s, titleId: %p, versionId: %p",
        (intptr_t*)inst, (intptr_t*)socketManager, address, titleId, versionId);

    std::string_view addressView{ address };
    if (const auto colon = addressView.find(':'); colon != std::string_view::npos)
    {
        const auto port = addressView.substr(colon + 1);
        address = g_game->adjustPeerAddress(address, port);
    }

    socketManager = (intptr_t)g_game->getSocketManager();

    return trampoline(inst, socketManager, address, titleId, versionId);
}

intptr_t onEvent(intptr_t m_client, intptr_t event)
{
    const auto trampoline = GG::HookManager::getManager().Call(&onEvent);
    auto* e = reinterpret_cast<std::uint32_t*>(event + 12);

    switch (*e)
    {
    case 0xBF585A6D: {
        if (GetOptionParameter("Client.ServerIp", "", 0)[0] != '\0')
        {
            *e = 0x4551EDBA;
            g_game->setJoining(true);
        }

        initSettings();

        void* user = reinterpret_cast<void*>(g_game->getPrimaryUser());
        UserAdded(nullptr, &user, 0);

        break;
    }

    case 0x1e86c1d6: {
        if (!splash)
        {
            auto gameSettings = GetSettings<GameSettings>(offsets::gw3::g_SettingsManager, offsets::gw3::g_GameSettings);
            auto level = "Levels/Level_Picnic_Root/Level_Picnic_Root";
            auto dsub = "Levels/DSub_SocialSpace/DSub_SocialSpace";
            auto start = "StartPoint_SocialSpace";
            auto inclusion = "GameMode=Mode_SocialSpace;HostedMode=PeerHosted";

            fbString(reinterpret_cast<intptr_t>(&gameSettings->Level), level, (int)strlen(level));
            fbString(reinterpret_cast<intptr_t>(&gameSettings->InitialDSubLevel), dsub, (int)strlen(dsub));
            fbString(reinterpret_cast<intptr_t>(&gameSettings->StartPoint), start, (int)strlen(start));
            fbString(reinterpret_cast<intptr_t>(&gameSettings->DefaultLayerInclusion), inclusion, (int)strlen(inclusion));
        }

        break;
    }

    default:
        break;
    }

    return trampoline(m_client, event);
}

intptr_t GetPlayerName(intptr_t name)
{
    *reinterpret_cast<char**>(name) = GetOptionParameter("name", "Player", 0);

    return name;
}

intptr_t CreateUser(intptr_t inst, int type, intptr_t handle, intptr_t alloc)
{
    const auto trampoline = GG::HookManager::getManager().Call(&CreateUser);
    auto ret = trampoline(inst, type, handle, alloc);

    g_game->setPrimaryUser(ret);

    return ret;
}

char initDedicatedServer(intptr_t inst)
{
    GG_LOG(GG::LogLevel::Debug, "fb::GameSimulation::initDedicatedServer(this: %p)", inst);

    LevelSetup setup{};
    LevelSetupInit(&setup);

    setup.Name = GetOptionParameter("level", "Levels/Level_Picnic_Root/Level_Picnic_Root", 0);
    setup.LevelManagerInitialLevel = GetOptionParameter("GameSettings.InitialDSubLevel", "Levels/DSub_SocialSpace/DSub_SocialSpace", 0);
    setup.LevelManagerStartPoint = GetOptionParameter("GameSettings.StartPoint", "StartPoint_SocialSpace", 0);
    auto inclusion = GetOptionParameter("GameSettings.DefaultLayerInclusion", "GameMode=Mode_SocialSpace;HostedMode=PeerHosted", 0);

    SetInclusionOptions(&setup, inclusion);

    for (int i = 0; i < (int)setup.InclusionOptions.size(); ++i)
    {
        const auto& opt = setup.InclusionOptions[i];
        const char* key = opt.m_criterion ? opt.m_criterion : "<null>";
        const char* value = opt.m_value ? opt.m_value : "<null>";

        GG_LOG(GG::LogLevel::Debug, "info->levelSetup->m_inclusionOptions[k:v] = %s:%s", key, value);
    }

    ServerSpawnInfo spawnInfo(setup);
    spawnInfo.isSinglePlayer = false;
    spawnInfo.isLocalHost = false;
    spawnInfo.isDedicated = true;
    spawnInfo.isCoop = false;
    spawnInfo.saveData.init(0);

    SpawnServer(inst + 0x08, &spawnInfo);

    return 1;
}

char initDataPlatform(fb::Main* inst)
{
    const auto trampoline = GG::HookManager::getManager().Call(&initDataPlatform);

    inst->m_isDedicatedServer = GetOptionParameter("server", nullptr, 0) != nullptr;

    return trampoline(inst);
}

intptr_t CreateGameWindow()
{
    const auto trampoline = GG::HookManager::getManager().Call(&CreateGameWindow);

    if (GetOptionParameter("server", nullptr, 0) != nullptr)
    {
        ApplyPatch(offsets::gw3::patch_NoWindow);
    }

    return trampoline();
}

char gameSimInit(fb::Main* inst, intptr_t info)
{
    const auto trampoline = GG::HookManager::getManager().Call(&gameSimInit);

    GG_LOG(GG::LogLevel::Debug, "[GameSim] Initializing Game Simulation");
    GG_LOG(GG::LogLevel::Debug, "gameSimInit(%p, %p)", inst, info);

    GG::HookManager::HookVTableFunction(reinterpret_cast<void*>(inst), &initDedicatedServer, 8);

    return trampoline(inst, info);
}

void MessageManagerDispatchMessage(void* inst, Message* message)
{
    const auto trampoline = GG::HookManager::getManager().Call(&MessageManagerDispatchMessage);

    if (message == nullptr)
    {
        trampoline(inst, message);
        return;
    }

    TypeInfo* type = message->getType();
    if (type == nullptr || type->typeInfoData == nullptr)
    {
        trampoline(inst, message);
        return;
    }

    auto name = type->getName();

    GG_LOG(GG::LogLevel::Debug, "[Message] %s, type: %d, category: %d, playerId: %d", name, message->type, message->category,
        message->LocalPlayerId);

    if (name != nullptr && std::strcmp(name, "ServerLevelLoadedMessage") == 0)
    {
        GG_LOG(GG::LogLevel::Info, "[SERVER] Level Loaded.");
    }

    if (name != nullptr && std::strcmp(name, "NetworkCreatePlayerMessage") == 0)
    {
        auto msg = reinterpret_cast<NetworkCreatePlayerMessage*>(message);

        GG_LOG(GG::LogLevel::Info, "[SERVER] %s has joined.", msg->m_name);
    }

    if (name != nullptr && std::strcmp(name, "ServerPlayerDisconnectMessage") == 0)
    {
        auto msg = reinterpret_cast<ServerPlayerDisconnectMessage*>(message);

        if (msg->m_player != nullptr)
        {
            GG_LOG(GG::LogLevel::Info, "[SERVER] %s has left.", msg->m_player->m_name);
        }
        else
        {
            GG_LOG(GG::LogLevel::Info, "[SERVER] Someone has left.");
        }
    }

    trampoline(inst, message);
}

void ServerLoadLevelMessagePost(LevelSetup* levelSetup, bool fadeOut, bool forceReloadResources)
{
    const auto trampoline = GG::HookManager::getManager().Call(&ServerLoadLevelMessagePost);

    GG_LOG(GG::LogLevel::Debug, "[ServerLoadLevelMessagePost] levelSetup->Name: %s", levelSetup->Name);
    GG_LOG(GG::LogLevel::Debug, "[ServerLoadLevelMessagePost] levelSetup->Startpoint: %s", levelSetup->StartPoint);
    GG_LOG(
        GG::LogLevel::Debug, "[ServerLoadLevelMessagePost] levelSetup->LevelManagerInitialLevel: %s", levelSetup->LevelManagerInitialLevel);
    GG_LOG(GG::LogLevel::Debug, "[ServerLoadLevelMessagePost] levelSetup->LevelManagerStartPoint: %s", levelSetup->LevelManagerStartPoint);

    trampoline(levelSetup, fadeOut, forceReloadResources);
}
} // namespace gw3
} // namespace fb

static GG::HookTemplate g_PvZGW1_Hooks[] = {
    { offsets::gw1::fn_InitSettings, reinterpret_cast<void*>(fb::gw1::initSettings), true },
    { offsets::gw1::fn_ServerStart, reinterpret_cast<void*>(fb::gw1::ServerStart), true },
    { offsets::gw1::fn_ClientInitNetwork, reinterpret_cast<void*>(fb::gw1::ClientInitNetwork), true },
    { offsets::gw1::fn_ClientConnectToAddress, reinterpret_cast<void*>(fb::gw1::ClientConnectToAddress), true },
    { offsets::gw1::fn_NetworkEnginePeerInit, reinterpret_cast<void*>(fb::gw1::NetworkEnginePeerInit), true },
    { offsets::gw1::fn_ClientInactivityTimer, reinterpret_cast<void*>(fb::gw1::ClientInactivityTimer), true },
    { offsets::gw1::fn_GetPlayerName, reinterpret_cast<void*>(fb::gw1::GetPlayerName), true },
    { offsets::gw1::fn_MainInit, reinterpret_cast<void*>(fb::gw1::MainInit), true },
    { offsets::gw1::fn_MessageManagerDispatchMessage, reinterpret_cast<void*>(fb::gw1::MessageManagerDispatchMessage), true },
};

static GG::HookTemplate g_PvZGW2_Hooks[] = {
    { offsets::gw2::fn_InitSettings, reinterpret_cast<void*>(fb::gw2::initSettings), true },
    { offsets::gw2::fn_ServerStart, reinterpret_cast<void*>(fb::gw2::ServerStart), true },
    { offsets::gw2::fn_ClientInitNetwork, reinterpret_cast<void*>(fb::gw2::ClientInitNetwork), true },
    { offsets::gw2::fn_ClientConnectToAddress, reinterpret_cast<void*>(fb::gw2::ClientConnectToAddress), true },
    { offsets::gw2::fn_NetworkEnginePeerInit, reinterpret_cast<void*>(fb::gw2::NetworkEnginePeerInit), true },
    { offsets::gw2::fn_GetPlayerName, reinterpret_cast<void*>(fb::gw2::GetPlayerName), true },
    { offsets::gw2::fn_MainInit, reinterpret_cast<void*>(fb::gw2::MainInit), true },
    { offsets::gw2::fn_MessageManagerExecuteMessage, reinterpret_cast<void*>(fb::gw2::MessageManagerExecuteMessage), true },
    { offsets::gw2::fn_ServerLoadLevelMessagePost, reinterpret_cast<void*>(fb::gw2::ServerLoadLevelMessagePost), true },
    { offsets::gw2::fn_getPlatformDataType, reinterpret_cast<void*>(fb::gw2::getPlatformDataType), true },
    { offsets::gw2::fn_getBackendType, reinterpret_cast<void*>(fb::gw2::getBackendType), true },
};

static GG::HookTemplate g_PvZGW3_Hooks[] = {
    { offsets::gw3::fn_ServerStart, reinterpret_cast<void*>(fb::gw3::ServerStart), true },
    { offsets::gw3::fn_ClientInitNetwork, reinterpret_cast<void*>(fb::gw3::ClientInitNetwork), true },
    { offsets::gw3::fn_ClientConnectToAddress, reinterpret_cast<void*>(fb::gw3::ClientConnectToAddress), true },
    { offsets::gw3::fn_NetworkEnginePeerInit, reinterpret_cast<void*>(fb::gw3::NetworkEnginePeerInit), true },
    { offsets::gw3::fn_OnEvent, reinterpret_cast<void*>(fb::gw3::onEvent), true },
    { offsets::gw3::fn_GetPlayerName, reinterpret_cast<void*>(fb::gw3::GetPlayerName), true },
    { offsets::gw3::fn_CreateUser, reinterpret_cast<void*>(fb::gw3::CreateUser), true },
    { offsets::gw3::fn_GameSimulationInit, reinterpret_cast<void*>(fb::gw3::gameSimInit), true },
    { offsets::gw3::fn_InitDataPlatform, reinterpret_cast<void*>(fb::gw3::initDataPlatform), true },
    { offsets::gw3::fn_CreateGameWindow, reinterpret_cast<void*>(fb::gw3::CreateGameWindow), true },
    { offsets::gw3::fn_MessageManagerDispatchMessage, reinterpret_cast<void*>(fb::gw3::MessageManagerDispatchMessage), true },
    { offsets::gw3::fn_ServerLoadLevelPost, reinterpret_cast<void*>(fb::gw3::ServerLoadLevelMessagePost), true },
};
