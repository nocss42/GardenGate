#pragma once
#include <list>
#include <cstdint>
#include <string_view>
#include <charconv>

#include "base/Log.h"
#include "ISocketAddress.h"
#include "UDPSocket.h"
#include "ISocketManager.h"
#include "IUDPSocketCreator.h"

namespace fb
{
namespace detail
{
inline bool parse_ipv4_and_port(const char* in, std::string_view& out_ip, std::uint16_t& out_port) noexcept
{
    if (!in)
        return false;

    std::string_view s(in);
    const auto colon = s.find(':');

    out_ip = (colon == std::string_view::npos) ? s : s.substr(0, colon);
    if (out_ip.empty())
        out_ip = "0.0.0.0";
    if (colon == std::string_view::npos)
        return false;

    const auto port_sv = s.substr(colon + 1);
    if (port_sv.empty())
        return false;

    unsigned port_u = 0;
    auto [ptr, ec] = std::from_chars(port_sv.data(), port_sv.data() + port_sv.size(), port_u, 10);
    if (ec != std::errc{} || ptr != port_sv.data() + port_sv.size() || port_u > 65535u)
        return false;

    out_port = static_cast<std::uint16_t>(port_u);
    return true;
}

template<typename SocketT, typename CreatorInterfaceT, typename ManagerT>
class SocketManagerImpl : public ISocketManager, public CreatorInterfaceT
{
protected:
    std::list<SocketT*> m_sockets;

public:
    SocketManagerImpl()
    {
        GG_LOG(GG::LogLevel::Debug, "Created new SocketManager");
    }

    __forceinline __int64 getArena()
    {
        return 0;
    }

    void Destroy() override
    {
        GG_LOG(GG::LogLevel::Debug, "Destroyed SocketManager");
        for (auto* s : m_sockets)
            if (s)
                s->Close();
        delete this;
    }

    void Close(SocketT* socket) override
    {
        m_sockets.remove(socket);
    }

    ISocket* Connect(const char*, bool = false) override
    {
        return nullptr;
    }

    ISocket* Listen(const char* name, bool blocking = false) override
    {
        std::string_view ip{};
        std::uint16_t port = 0;

        if (!parse_ipv4_and_port(name, ip, port))
        {
            GG_LOG(GG::LogLevel::Error, "Invalid listen address: '%s'", name ? name : "(null)");
            return nullptr;
        }

        auto* socket = new SocketT(static_cast<ManagerT*>(this));
        const ISocketAddress bind_addr{ ip, port };

        if (!socket->Listen(bind_addr, blocking))
        {
            GG_LOG(GG::LogLevel::Error, "Listen failed %.*s:%u", (int)ip.size(), ip.data(), (unsigned)port);
            delete socket;
            return nullptr;
        }

        GG_LOG(GG::LogLevel::Info, "SocketManager listening on %.*s:%u", (int)ip.size(), ip.data(), (unsigned)port);
        m_sockets.push_back(socket);
        return reinterpret_cast<ISocket*>(socket);
    }

    ISocket* CreateSocket() override
    {
        return nullptr;
    }

    ~SocketManagerImpl() = default;
};
} // namespace detail

class SocketManager final : public detail::SocketManagerImpl<UDPSocket, IUDPSocketCreator, SocketManager>
{};

namespace gw3
{
class SocketManager final : public detail::SocketManagerImpl<gw3::UDPSocket, gw3::IUDPSocketCreator, gw3::SocketManager>
{};
} // namespace gw3
} // namespace fb
