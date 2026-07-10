#pragma once

#include "sdk/SocketManager.h"

namespace fb
{
class SocketManagerFactory
{
public:
    virtual SocketManager* createSocketManager()
    {
        return new SocketManager();
    }
};

namespace gw3
{
class SocketManagerFactory
{
public:
    virtual SocketManager* createSocketManager()
    {
        return new SocketManager();
    }
};
} // namespace gw3
} // namespace fb
