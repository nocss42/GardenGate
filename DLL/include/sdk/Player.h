#pragma once

#include "sdk/Types.h"

namespace fb
{
enum TeamId : __int32
{
    TeamNeutral = 0x0,
    Team1 = 0x1,
    Team2 = 0x2,
    Team3 = 0x3,
    Team4 = 0x4,
    Team5 = 0x5,
    Team6 = 0x6,
    Team7 = 0x7,
    Team8 = 0x8,
    Team9 = 0x9,
    Team10 = 0xA,
    Team11 = 0xB,
    Team12 = 0xC,
    Team13 = 0xD,
    Team14 = 0xE,
    Team15 = 0xF,
    Team16 = 0x10,
    TeamIdCount = 0x11,
};

struct OnlineId
{
    uint64_t m_nativeData;
    char m_id[17];
};

struct Player
{
    void* __vftable;
    void* m_data;
    void** m_memoryArena;
    char* m_name;
    OnlineId m_onlineId;
    OnlineId m_groupId;
    OnlineId m_clubId;
    TeamId m_teamPreference;
    unsigned int m_lastVehicleCameraIndex;
    unsigned int m_analogInputEnableMask;
    unsigned __int64 m_digitalInputEnableMask;
    void* m_selectedCustomizationAsset;
    void* m_selectedActiveUnlocks;
    bool m_isAIPlayer;
    bool m_isSpectator;
    TeamId m_teamId;
};

class ServerPlayer
{
public:
    void* __vftable;
    intptr_t m_data;
    intptr_t m_memoryArena;
    const char* m_name;
};

namespace gw3
{
class ServerPlayer
{
public:
    char __pad[0x20];
    const char m_name[0x18];
};
} // namespace gw3
} // namespace fb