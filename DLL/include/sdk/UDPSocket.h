#pragma once

#include <cstdint>

#include "sdk/ISocket.h"
#include "sdk/ISocketAddress.h"
#include "sdk/IUDPSocket.h"
#include "base/Log.h"

#include <winsock2.h>
#define _WINSOCKAPI_
#include <windows.h>
#include <ws2tcpip.h>

namespace fb
{
class SocketManager;
namespace gw3
{
class SocketManager;
}

namespace detail
{
template<typename InterfaceT, typename ManagerT>
class UDPSocketImpl : public InterfaceT
{
protected:
    ManagerT* m_creator{ nullptr };
    SOCKET m_socket{ INVALID_SOCKET };
    int m_port{ 0 };
    bool m_blocking{ false };
    bool m_peerValid{ false };
    ISocketAddress m_address{};
    ISocketAddress m_peerAddress{};

public:
    explicit UDPSocketImpl(ManagerT* creator) noexcept
        : m_creator(creator)
    {}

    virtual ~UDPSocketImpl()
    {
        Close();
    }

    void Close()
    {
        if (m_socket != INVALID_SOCKET)
        {
            ::closesocket(m_socket);
            m_socket = INVALID_SOCKET;
            GG_LOG(GG::LogLevel::Info, "UDP socket closed (port %d)", m_port);
        }
    }

    bool Send(uint8_t* buffer, int size, unsigned int flags = 0) override
    {
        if (!m_peerValid)
            return false;

        const sockaddr* addr = reinterpret_cast<const sockaddr*>(m_peerAddress.data());

        const int ret = ::sendto(m_socket, reinterpret_cast<const char*>(buffer), size, flags, addr, sizeof(sockaddr_in));

        if (ret < 0)
        {
            const int err = ::WSAGetLastError();
            if (err != WSAEWOULDBLOCK)
                GG_LOG(GG::LogLevel::Warning, "sendto failed (%d)", err);
            return false;
        }

        return true;
    }

    int ReceiveFrom(uint8_t* buffer, int bufferSize) override
    {
        sockaddr_in from{};
        int fromLen = sizeof(from);

        const int ret = ::recvfrom(m_socket, reinterpret_cast<char*>(buffer), bufferSize, 0, reinterpret_cast<sockaddr*>(&from), &fromLen);

        if (ret < 0)
        {
            const int err = ::WSAGetLastError();
            if (err != WSAEWOULDBLOCK)
                GG_LOG(GG::LogLevel::Debug, "recvfrom error (%d)", err);
            return ret;
        }

        m_peerAddress.set_data(&from, sizeof(from));
        m_peerValid = true;

        return ret;
    }

    bool SetBroadcast(uint16_t /*port*/) override
    {
        int enable = 1;
        return ::setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&enable), sizeof(enable)) == 0;
    }

    void SetPeerAddress(const ISocketAddress& addr) override
    {
        m_peerAddress = addr;
        m_peerValid = true;
    }

    ISocketAddress PeerAddress() const override
    {
        return m_peerAddress;
    }

    bool SetDefaultPacketInfo(typename InterfaceT::PacketInfo* info) override
    {
        if (!info)
            return false;
        info->minSize = 1;
        info->maxSize = 1264;
        info->recommendedSize = info->maxSize;
        info->overheadWhenAligned = 28;
        info->alignment = 1;
        return true;
    }

    bool BlockingMode() const override
    {
        return m_blocking;
    }

    bool SetBlockingMode(__int64 blocking) override
    {
        if (m_socket == INVALID_SOCKET)
            return false;

        u_long mode = blocking ? 0 : 1;
        if (::ioctlsocket(m_socket, FIONBIO, &mode) != 0)
            return false;

        m_blocking = blocking != 0;
        return true;
    }

    void ReceivePulse() override {}
    void Pulse(float) override {}
    void SendPulse() override {}

    intptr_t NativeSocket() const override
    {
        return static_cast<intptr_t>(m_socket);
    }

    int Port() const override
    {
        return m_port;
    }

    const ISocketAddress* Address() const override
    {
        return &m_address;
    }

    void SendProxyHandshake() override {}

    bool Connect(const ISocketAddress& address, bool blocking = false) override
    {
        (void)address;
        (void)blocking;
        return false;
    }

    bool Listen(const ISocketAddress& address, bool blocking = false) override
    {
        Close();

        m_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (m_socket == INVALID_SOCKET)
        {
            GG_LOG(GG::LogLevel::Error, "Failed to create UDP socket");
            return false;
        }

        if (::bind(m_socket, reinterpret_cast<const sockaddr*>(address.data()), sizeof(sockaddr_in)) != 0)
        {
            GG_LOG(GG::LogLevel::Error, "bind failed (%d)", WSAGetLastError());
            Close();
            return false;
        }

        SetBlockingMode(blocking);

        const auto* a = reinterpret_cast<const sockaddr_in*>(address.data());
        m_port = ::ntohs(a->sin_port);
        m_address = address;

        GG_LOG(GG::LogLevel::Info, "UDP socket listening on port %d", m_port);
        return true;
    }

    bool Create(bool blocking = false) override
    {
        Close();

        m_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (m_socket == INVALID_SOCKET)
            return false;

        return SetBlockingMode(blocking);
    }
};
} // namespace detail

class UDPSocket final : public detail::UDPSocketImpl<IUDPSocket, SocketManager>
{
public:
    explicit UDPSocket(SocketManager* creator) noexcept
        : detail::UDPSocketImpl<IUDPSocket, SocketManager>(creator)
    {}
};

namespace gw3
{
class UDPSocket final : public detail::UDPSocketImpl<gw3::IUDPSocket, gw3::SocketManager>
{
public:
    explicit UDPSocket(gw3::SocketManager* creator) noexcept
        : detail::UDPSocketImpl<gw3::IUDPSocket, gw3::SocketManager>(creator)
    {}

    int ReceiveFromWhen(uint8_t* buffer, int maxSize, unsigned int& receivedWhen) override
    {
        receivedWhen = 0;
        return this->ReceiveFrom(buffer, maxSize);
    }
};
} // namespace gw3
} // namespace fb
