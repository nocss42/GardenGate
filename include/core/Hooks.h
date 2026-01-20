#pragma once

#include <sstream>

#include "base/Log.h"
#include "core/HookManager.h"
#include "core/Game.h"
#include "core/Offsets.h"
#include "sdk/Types.h"
#include "sdk/ServerSpawnInfo.h"
#include "sdk/ServerSpawnOverrides.h"
#include "sdk/SocketManagerFactory.h"
#include "sdk/SecureReason.h"
#include "sdk/ScriptContext.h"
#include "sdk/vector.h"
#include "sdk/Settings.h"

static bool splash = false;

namespace fb
{
    // ============================================================================
    // PvZ: Garden Warfare 1
    // ============================================================================
    namespace gw1
    {
        using tGetOptionParameter = char* (*)(const char*, const char*, int*);
        inline tGetOptionParameter GetOptionParameter =
            reinterpret_cast<tGetOptionParameter>(offsets::gw1::fn_GetOptionParameter);

        __int64 ServerStart(intptr_t inst, ServerSpawnInfo& info, ServerSpawnOverrides* spawnOverrides)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&ServerStart);

            g_game->logServerSpawnInfo(info);
            g_game->prepareServerSpawn(inst, info, spawnOverrides);
            
            info.isCoop = false;

            return trampoline(inst, info, spawnOverrides);
        }

        bool ClientInitNetwork(intptr_t inst, bool singleplayer, bool localhost, bool coop, bool hosted)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&ClientInitNetwork);

            g_game->logClientInitNetwork(singleplayer, localhost, coop, hosted);

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

        char NetworkEnginePeerInit(intptr_t inst, __int64 crypto, intptr_t socketManager, 
            const char* address, int titleId, int versionId, bool debug, bool voice, bool live)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&NetworkEnginePeerInit);

            std::string_view addressView{ address };
            if (const auto colon = addressView.find(':'); colon != std::string_view::npos)
            {
                const auto port = addressView.substr(colon + 1);
                address = g_game->adjustPeerAddress(address, port);
            }

            return trampoline(inst, crypto, socketManager, address, titleId, versionId, debug, voice, live);
        }

        void PeerHasJoined(intptr_t inst, intptr_t playerInstance, const char* player)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&PeerHasJoined);

            if (strlen(player) > 0)
                g_game->logPeerJoined(player);

            trampoline(inst, playerInstance, player);
        }

        void ClientDisconnect(intptr_t inst)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&ClientDisconnect);

            SecureReason reason = *reinterpret_cast<const SecureReason*>(inst + 0x670);
            g_game->handleDisconnection(reason);

            trampoline(inst);
        }

        void __fastcall ClientInactivityTimer(intptr_t, float)
        {
            return;
        }

        char* GetPlayerName()
        {
            return GetOptionParameter("name", "Player", 0);
        }
    }

    // ============================================================================
    // PvZ: Garden Warfare 2
    // ============================================================================
    namespace gw2
    {
        using tGetOptionParameter = char* (*)(const char*, const char*, int*);
        inline tGetOptionParameter GetOptionParameter = 
            reinterpret_cast<tGetOptionParameter>(offsets::gw2::fn_GetOptionParameter);

        __int64 ServerStart(intptr_t inst, ServerSpawnInfo& info, ServerSpawnOverrides* spawnOverrides)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&ServerStart);

            g_game->logServerSpawnInfo(info);
            g_game->prepareServerSpawn(inst, info, spawnOverrides);

            // GW2 doesn't force isCoop to false (unlike GW1)

            return trampoline(inst, info, spawnOverrides);
        }

        bool ClientInitNetwork(intptr_t inst, char idk, bool localhost, bool coop, bool hosted, bool singleplayer)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&ClientInitNetwork);

            GG_LOG(GG::LogLevel::Info, 
                "fb::client::InitNetwork(idk: %d, singleplayer: %d, localhost: %d, coop: %d, hosted: %d)",
                (char)idk, (char)singleplayer, (char)localhost, (char)coop, (char)hosted);

            if (!localhost)
            {
                g_game->injectSocketManagerFactory(inst, 0xB8);
                
                singleplayer = false;
                coop = false;
                hosted = true;
            }

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

        char NetworkEnginePeerInit(intptr_t inst, __int64 crypto, intptr_t socketManager, 
            const char* address, int titleId, int versionId)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&NetworkEnginePeerInit);

            std::string_view addressView{ address };
            if (const auto colon = addressView.find(':'); colon != std::string_view::npos)
            {
                const auto port = addressView.substr(colon + 1);
                address = g_game->adjustPeerAddress(address, port);
            }

            return trampoline(inst, crypto, socketManager, address, titleId, versionId);
        }

        __int64 ClientDisconnected(intptr_t inst, SecureReason reason, char* reasonText)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&ClientDisconnected);

            g_game->handleDisconnection(reason);

            return trampoline(inst, reason, reasonText);
        }

        void PeerHasJoined(intptr_t inst, intptr_t playerInstance, const char* player)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&PeerHasJoined);

            if (strlen(player) > 0)
                g_game->logPeerJoined(player);

            trampoline(inst, playerInstance, player);
        }

        char* GetPlayerName()
        {
            return GetOptionParameter("name", "Player", 0);
        }
    }

    // ============================================================================
    // PvZ:Battle for Neighborville
    // ============================================================================
    namespace gw3 {
        using tGetOptionParameter = char* (*)(const char*, const char*, int*);
        inline tGetOptionParameter GetOptionParameter =
            reinterpret_cast<tGetOptionParameter>(offsets::gw3::fn_GetOptionParameter);

        using tGetSettings = intptr_t(*)(intptr_t, intptr_t);

        template <typename T>
        inline T* GetSettings(intptr_t settingsManager, intptr_t settingsInstance)
        {
            auto fn = reinterpret_cast<tGetSettings>(offsets::gw3::fn_GetSettings);

            intptr_t mgr = *reinterpret_cast<intptr_t*>(settingsManager);
            intptr_t ret = fn(mgr, settingsInstance);
            return reinterpret_cast<T*>(ret);
        }

        intptr_t ServerStart(intptr_t inst, ServerSpawnInfo& info, ServerSpawnOverrides* spawnOverrides, fb::ISocketManager* socketManager) {
            const auto trampoline = GG::HookManager::getManager().Call(&ServerStart);

            GG_LOG(
                GG::LogLevel::Debug,
                "ServerStart - inst:%p, info:%p, spawnOverrides:%p, socketManager:%p",
                (intptr_t*)inst,
                (intptr_t*)&info,
                (intptr_t*)spawnOverrides,
                (intptr_t*)socketManager
            );

            fb::gw3::LevelSetup* levelSetup = reinterpret_cast<fb::gw3::LevelSetup*>(&info.levelSetup);

            GG_LOG(GG::LogLevel::Debug, "info->levelSetup->m_name = %s", levelSetup->Name);

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

            g_game->prepareServerSpawn(inst, info, spawnOverrides);


            if (!levelName.starts_with("Levels/Level_Picnic_Splash/Level_Picnic_Splash")) {
                GG_LOG(GG::LogLevel::Debug, "Forcing non-localhost");
                g_game->setHosting(true);

                info.isLocalHost = false;

                spawnOverrides->socketManager = g_game->getSocketManager();
            }
            else {
                ::splash = true;
            }


            return trampoline(inst, info, spawnOverrides, socketManager);
        }

        bool ClientInitNetwork(intptr_t inst, bool singleplayer, bool localhost, bool coop, bool hosted)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&ClientInitNetwork);

            g_game->logClientInitNetwork(singleplayer, localhost, coop, hosted);

            if (::splash)
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

            if (g_game->isJoining()) {
                ipAddress = GetOptionParameter("Client.ServerIp", "", 0);
            }

            g_game->logClientConnect(ipAddress, serverPassword);

            return trampoline(inst, ipAddress, serverPassword);
        }

        char NetworkEnginePeerInit(intptr_t inst, intptr_t socketManager,
            const char* address, int titleId, int versionId)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&NetworkEnginePeerInit);

            GG_LOG(
                GG::LogLevel::Debug,
                "NetworkEnginePeerInit - inst:%p, socketManager:%p, address: %s, titleId: %p, versionId: %p",
                (intptr_t*)inst,
                (intptr_t*)socketManager,
                address,
                titleId,
                versionId
            );

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

            switch (*e) {
                case 0xBF585A6D: {
                    if (GetOptionParameter("Client.ServerIp", "", 0)[0] != '\0') {
                        *e = 0x4551EDBA;
                        g_game->setJoining(true);
                    }

                    auto netObjectSettings = GetSettings<NetObjectSystemSettings>(offsets::gw3::g_SettingsManager, offsets::gw3::g_NetObjectSettings);
                    netObjectSettings->MaxServerConnectionCount = 64;

                    auto serverSettings = GetSettings<PVZServerSettings>(offsets::gw3::g_SettingsManager, offsets::gw3::g_PVZServerSettings);
                    serverSettings->KickIdlePlayers = false;

                    auto pvzOnlineSettings = GetSettings<PVZOnlineSettings>(offsets::gw3::g_SettingsManager, offsets::gw3::g_PVZOnlineSettings);
                    pvzOnlineSettings->ClientIsPresenceEnabled = false;
                    pvzOnlineSettings->ServerAllowAnyReputation = true;

                    auto gameModeSettings = GetSettings<GameModeSettings>(offsets::gw3::g_SettingsManager, offsets::gw3::g_GameModeSettings);
                    gameModeSettings->ShouldSkipHUBTutorial = true;
                    gameModeSettings->SocialHUBSkipStationTutorials = true;
                    gameModeSettings->SocialHUBSkipLandingPage = true;


                    break;
                }

                case 0x2ED8C3: {
                    if (g_game->isJoining()) {
                        g_game->handleDisconnection(SecureReason_NoReply);
                    }

                    break;
                }

                default:
                    break;
            }

            return trampoline(m_client, event);
        }

        void PeerHasJoined(intptr_t inst, intptr_t playerInstance, const char* player)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&PeerHasJoined);

            if (strlen(player) > 0)
                g_game->logPeerJoined(player);

            trampoline(inst, playerInstance, player);
        }

        intptr_t GetPlayerName(intptr_t name)
        {
            *reinterpret_cast<char**>(name) = GetOptionParameter("name", "Player", 0);

            return name;
        }


                void ApplySettings(intptr_t inst, ScriptContext* scriptContext, bool inmediate)
        {
            const auto trampoline = GG::HookManager::getManager().Call(&ApplySettings);

            void* poolBase[25];

            using tGetAllUsers = void(*)(void*, void**);
            auto getAllUsers = reinterpret_cast<tGetAllUsers>(offsets::gw3::fn_GetUsers);

            getAllUsers(*reinterpret_cast<void**>(offsets::gw3::g_PairingManager), poolBase);

            if (*poolBase != poolBase)
            {
                void* primaryUser = *reinterpret_cast<void**>(reinterpret_cast<char*>(*poolBase) + 0x10);

                using tAddPermanentUser = void(*)(void*, void**, unsigned int);
                auto addPermanentUser = reinterpret_cast<tAddPermanentUser>(offsets::gw3::fn_AddPermaUser);

               addPermanentUser(nullptr, &primaryUser, 0);
            }

            auto* options = reinterpret_cast<eastl::vector<const char*>*>(offsets::gw3::g_OptionsVec);

            if (!options->empty())
            {
                std::stringstream ss_applySettings;

                ss_applySettings << "local function applySettings(settings)\n"
                    "core.parseKeyValueCfgString(settings, _G)\n"
                      "end\n"
                    "applySettings [=[\n";

                for (int o = 0; o < options->size(); o++)
                {
                    const auto option = options->at(o);

                    if (o + 1 >= options->size()) continue;

                    const auto value = options->at(o + 1);

                    if (*value != '-')
                        ss_applySettings << option + 1 << " " << value << "\n";
                }

                ss_applySettings << "]=]";

                scriptContext->executeString(ss_applySettings.str().c_str());
            }

            trampoline(inst, scriptContext, inmediate);
        }

    }

        

}



static GG::HookTemplate g_PvZGW1_Hooks[] = {
    {offsets::gw1::fn_ServerStart,              reinterpret_cast<void*>(fb::gw1::ServerStart),              true},
    {offsets::gw1::fn_ClientInitNetwork,        reinterpret_cast<void*>(fb::gw1::ClientInitNetwork),        true},
    {offsets::gw1::fn_ClientConnectToAddress,   reinterpret_cast<void*>(fb::gw1::ClientConnectToAddress),   true},
    {offsets::gw1::fn_NetworkEnginePeerInit,    reinterpret_cast<void*>(fb::gw1::NetworkEnginePeerInit),    true},
    {offsets::gw1::fn_PeerHasJoined,            reinterpret_cast<void*>(fb::gw1::PeerHasJoined),            true},
    {offsets::gw1::fn_ClientDisconnect,         reinterpret_cast<void*>(fb::gw1::ClientDisconnect),         true},
    {offsets::gw1::fn_ClientInactivityTimer,    reinterpret_cast<void*>(fb::gw1::ClientInactivityTimer),    true},
    {offsets::gw1::fn_GetPlayerName,            reinterpret_cast<void*>(fb::gw1::GetPlayerName),            true},
};

static GG::HookTemplate g_PvZGW2_Hooks[] = {
    {offsets::gw2::fn_ServerStart,              reinterpret_cast<void*>(fb::gw2::ServerStart),              true},
    {offsets::gw2::fn_ClientInitNetwork,        reinterpret_cast<void*>(fb::gw2::ClientInitNetwork),        true},
    {offsets::gw2::fn_ClientConnectToAddress,   reinterpret_cast<void*>(fb::gw2::ClientConnectToAddress),   true},
    {offsets::gw2::fn_NetworkEnginePeerInit,    reinterpret_cast<void*>(fb::gw2::NetworkEnginePeerInit),    true},
    {offsets::gw2::fn_ClientDisconnected,       reinterpret_cast<void*>(fb::gw2::ClientDisconnected),       true},
    {offsets::gw2::fn_PeerHasJoined,            reinterpret_cast<void*>(fb::gw2::PeerHasJoined),            true},
    {offsets::gw2::fn_GetPlayerName,            reinterpret_cast<void*>(fb::gw2::GetPlayerName),            true},
};

static GG::HookTemplate g_PvZGW3_Hooks[] = {
    {offsets::gw3::fn_ServerStart,              reinterpret_cast<void*>(fb::gw3::ServerStart),              true},
    {offsets::gw3::fn_ClientInitNetwork,        reinterpret_cast<void*>(fb::gw3::ClientInitNetwork),        true},
    {offsets::gw3::fn_ClientConnectToAddress,   reinterpret_cast<void*>(fb::gw3::ClientConnectToAddress),   true},
    {offsets::gw3::fn_ApplySettings,            reinterpret_cast<void*>(fb::gw3::ApplySettings),            true},
    {offsets::gw3::fn_NetworkEnginePeerInit,    reinterpret_cast<void*>(fb::gw3::NetworkEnginePeerInit),    true},
    {offsets::gw3::fn_OnEvent,                  reinterpret_cast<void*>(fb::gw3::onEvent),                  true},
    {offsets::gw3::fn_PeerHasJoined,            reinterpret_cast<void*>(fb::gw3::PeerHasJoined),            true},
    {offsets::gw3::fn_GetPlayerName,            reinterpret_cast<void*>(fb::gw3::GetPlayerName),            true},
};