#pragma once

#include "ISocket.h"
#include "ISocketAddress.h"

namespace fb
{
struct IUDPSocket : ISocket
{
    virtual bool Connect(const ISocketAddress& address, bool blocking = false) = 0;
    virtual bool Listen(const ISocketAddress& address, bool blocking = false) = 0;
    virtual bool Create(bool blocking = false) = 0;
};

namespace gw3
{
struct IUDPSocket : ISocket
{
    virtual bool Connect(const ISocketAddress& address, bool blocking = false) = 0;
    virtual bool Listen(const ISocketAddress& address, bool blocking = false) = 0;
    virtual bool Create(bool blocking = false) = 0;
};
} // namespace gw3
} // namespace fb
