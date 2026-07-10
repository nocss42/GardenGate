#pragma once

namespace fb
{
class UDPSocket;
class IUDPSocketCreator
{
public:
    virtual void Close(UDPSocket* socket) = 0;
};

namespace gw3
{
class UDPSocket;
class IUDPSocketCreator
{
public:
    virtual void Close(UDPSocket* socket) = 0;
};
} // namespace gw3
} // namespace fb