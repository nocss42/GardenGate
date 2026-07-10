#pragma once

#include "sdk/Types.h"
#include "sdk/Player.h"

namespace fb
{
const struct Message : ITypedObject
{
    const int category;
    const int type;
    const int LocalPlayerId;
    long double m_dispatchTime;
    const fb::Message* m_next;
    int m_postedAtProcessMessageCounter;
    bool m_ownedByMessageManager;
};

struct NetworkableMessage : Message
{
    void* m_serverConnection;
    void* m_clientConnection;
    volatile unsigned int* m_syncObject;
};

struct NetworkCreatePlayerMessage : NetworkableMessage
{
    char* m_name;
    bool m_isSpectator;
};

class ServerPlayerDisconnectMessage : NetworkableMessage
{
public:
    class ServerPlayer* m_player;
};

struct ServerPlayerCreateMessage : Message
{
    const Player* m_player;
};

struct ServerPlayerDestroyMessage : Message
{
    const Player* m_player;
};

struct NetworkDisconnectedMessage : Message
{
    SecureReason m_reason;
};

namespace gw3
{
struct NetworkCreatePlayerMessage : Message
{
    char __pad[0x3C];
    char m_name[0x20];
};

struct ServerPlayerDisconnectMessage : Message
{
    const ServerPlayer* m_player;
};
} // namespace gw3
} // namespace fb