#pragma once

#include "sdk/ISocket.h"

namespace fb
{
class ISocketManager
{
public:
    virtual void Destroy() = 0;
    virtual ISocket* Connect(const char* address, bool blocking = false) = 0;
    virtual ISocket* Listen(const char* address, bool blocking = false) = 0;
    virtual ISocket* CreateSocket() = 0;
};
} // namespace fb
