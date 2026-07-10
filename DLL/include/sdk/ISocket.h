#pragma once

#include "Array.h"
#include "Types.h"
#include "ISocketAddress.h"

namespace fb
{
struct ISocket
{
    struct PacketInfo
    {
        int minSize;
        int maxSize;
        int recommendedSize;
        int overheadWhenAligned;
        int alignment;
    };

    virtual ~ISocket() = default;

    virtual bool Send(uint8_t* sendBuffer, int size, unsigned int flags = 0) = 0;
    virtual int ReceiveFrom(uint8_t* buffer, int bufferSize) = 0;
    virtual bool SetBroadcast(uint16_t port) = 0;
    virtual void SetPeerAddress(const ISocketAddress& sockAddr) = 0;
    virtual ISocketAddress PeerAddress() const = 0;
    virtual bool SetDefaultPacketInfo(PacketInfo* info) = 0;
    virtual bool BlockingMode() const = 0;
    virtual bool SetBlockingMode(__int64 blocking) = 0;
    virtual void ReceivePulse() = 0;
    virtual void Pulse(float seconds) = 0;
    virtual void SendPulse() = 0;
    virtual intptr_t NativeSocket() const = 0;
    virtual int Port() const = 0;
    virtual const ISocketAddress* Address() const = 0;
    virtual void SendProxyHandshake() = 0;
};

namespace gw3
{
struct ISocket
{
    struct PacketInfo
    {
        int minSize;
        int maxSize;
        int recommendedSize;
        int overheadWhenAligned;
        int alignment;
    };

    virtual ~ISocket() = default;

    virtual bool Send(uint8_t* sendBuffer, int size, unsigned int flags = 0) = 0;
    virtual int ReceiveFrom(uint8_t* buffer, int bufferSize) = 0;
    virtual int ReceiveFromWhen(uint8_t* receiveBuffer, int maxSize, unsigned int& receivedWhen) = 0;
    virtual bool SetBroadcast(uint16_t port) = 0;
    virtual void SetPeerAddress(const ISocketAddress& sockAddr) = 0;
    virtual ISocketAddress PeerAddress() const = 0;
    virtual bool SetDefaultPacketInfo(PacketInfo* info) = 0;
    virtual bool BlockingMode() const = 0;
    virtual bool SetBlockingMode(__int64 blocking) = 0;
    virtual void ReceivePulse() = 0;
    virtual void Pulse(float seconds) = 0;
    virtual void SendPulse() = 0;
    virtual intptr_t NativeSocket() const = 0;
    virtual int Port() const = 0;
    virtual const ISocketAddress* Address() const = 0;
    virtual void SendProxyHandshake() = 0;
};
} // namespace gw3
} // namespace fb
