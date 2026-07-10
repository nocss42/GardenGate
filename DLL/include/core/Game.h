#pragma once

#include <memory>

#include "base/Log.h"
#include "core/Offsets.h"
#include "sdk/SocketManager.h"
#include "sdk/ServerSpawnInfo.h"
#include "sdk/ServerSpawnOverrides.h"
#include "sdk/SocketManagerFactory.h"
#include "sdk/SecureReason.h"
#include "sdk/LevelSetupOption.h"

namespace GG
{
enum class GameVersion
{
    GW1,
    GW2,
    GW3
};

class Game
{
public:
    void initialize(GameVersion version)
    {
        m_version = version;
        m_socketManager = (version == GameVersion::GW3) ? static_cast<fb::ISocketManager*>(new fb::gw3::SocketManager())
                                                        : static_cast<fb::ISocketManager*>(new fb::SocketManager());
    }

    void uninitialize();

    void onServerCreate(intptr_t inst)
    {
        m_serverInst = inst;
    }

    fb::ISocketManager* getSocketManager()
    {
        return m_socketManager;
    }
    bool isJoiningOrHosting()
    {
        return m_hosting || m_joining;
    }
    bool isHosting()
    {
        return m_hosting;
    }
    bool isJoining()
    {
        return m_joining;
    }
    void setJoining(bool joining)
    {
        m_joining = joining;
    }
    void setHosting(bool hosting)
    {
        m_hosting = hosting;
    }

    void injectSocketManagerFactory(intptr_t inst, std::size_t offset)
    {
        void* factory = (m_version == GameVersion::GW3) ? static_cast<void*>(new fb::gw3::SocketManagerFactory())
                                                        : static_cast<void*>(new fb::SocketManagerFactory());
        *reinterpret_cast<__int64*>(inst + offset) = reinterpret_cast<__int64>(factory);
    }

    const char* redirectHostingAddress(const char* ipAddress)
    {
        if (isHosting())
            return "127.0.0.1:25200";
        return ipAddress;
    }

    void logClientConnect(const char* ipAddress, const char* serverPassword)
    {
        GG_LOG(LogLevel::Info, "fb::client::ConnectToAddress(ipAddress: %s, serverPassword: %s)", ipAddress, serverPassword);
    }

    const char* adjustPeerAddress(const char* address, std::string_view port)
    {
        if (!isJoiningOrHosting())
            return address;

        if (port == "25200")
            return "0.0.0.0:25200";
        if (port == "25100")
            return "0.0.0.0:25100";

        return address;
    }

    void handleDisconnection(fb::SecureReason reason)
    {
        if (reason == fb::SecureReason_Ok)
            return;

        const auto msg = fb::secure_reason_message(reason);

        GG_LOG(LogLevel::Warning, "Disconnected[%d] - %.*s", (int)reason, (int)msg.size(), msg.data());

        if (!isHosting())
        {
            if (!fb::show_disconnected_dialog(reason))
            {
                std::exit(EXIT_FAILURE);
            }
        }
    }

    void logPeerJoined(const char* player)
    {
        GG_LOG(LogLevel::Info, "Peer joined - %s", player);
    }

    void setPrimaryUser(intptr_t user)
    {
        m_primaryUser = user;
    }

    intptr_t getPrimaryUser()
    {
        return m_primaryUser;
    }

private:
    GameVersion m_version;
    intptr_t m_serverInst{ 0 };
    intptr_t m_primaryUser{ 0 };
    bool m_hosting{ false };
    bool m_joining{ false };
    fb::ISocketManager* m_socketManager{ nullptr };
};
} // namespace GG

inline std::unique_ptr<GG::Game> g_game = nullptr;
