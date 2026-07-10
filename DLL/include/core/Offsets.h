#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "core/Util.h"

#define DEFINE_INSTANCE(name, addr) inline constexpr std::uintptr_t g_##name = (addr)
#define DEFINE_FUNCTION(name, addr) inline constexpr std::uintptr_t fn_##name = (addr)
#define DEFINE_PATCH(name, addr, data)                                                                                                     \
    inline constexpr auto patch_##name = Patch<std::tuple_size_v<decltype(data)>>                                                          \
    {                                                                                                                                      \
        (addr), (data)                                                                                                                     \
    }

namespace offsets
{
namespace gw1
{
//--- PvZ: GW1
DEFINE_INSTANCE(AppId, 0x141578300);
DEFINE_INSTANCE(SettingsManager, 0x141EAC0B0);

//--- Utility
DEFINE_FUNCTION(GetOptionParameter, 0x1403A97D0);
DEFINE_FUNCTION(SetInclusionOptions, 0x1404FA9F0);
DEFINE_FUNCTION(GetContainer, 0x1403BCFE0);
DEFINE_FUNCTION(LevelSetupInit, 0x141464B10);

//--- Hooks
DEFINE_FUNCTION(InitSettings, 0x14000D030);
DEFINE_FUNCTION(MainInit, 0x14000B070);
DEFINE_FUNCTION(ServerStart, 0x1406F8F80);
DEFINE_FUNCTION(CreateServer, 0x140011AE0);
DEFINE_FUNCTION(ClientInitNetwork, 0x140541740);
DEFINE_FUNCTION(ClientConnectToAddress, 0x140559CD0);
DEFINE_FUNCTION(NetworkEnginePeerInit, 0x1408038F0);
DEFINE_FUNCTION(ClientInactivityTimer, 0x140C00470);
DEFINE_FUNCTION(GetPlayerName, 0x1403A9170);
DEFINE_FUNCTION(MessageManagerDispatchMessage, 0x14035AD00);

DEFINE_PATCH(BackendOnMessage, 0x140B2D2A0, (std::array<std::uint8_t, 1>{ 0xC3 }));
DEFINE_PATCH(AllowMultipleInstances, 0x1400108B1, (std::array<std::uint8_t, 1>{ 0xEB }));
DEFINE_PATCH(AllowCommandArgumentsAsOptions, 0x14000D3FA, (std::array<std::uint8_t, 1>{ 0xDE }));
DEFINE_PATCH(DisableTutorials, 0x140B4142A, (std::array<std::uint8_t, 5>{ 0x90, 0x90, 0x90, 0x90, 0x90 }));
DEFINE_PATCH(FixAvailableConsumables, 0x140B3F69D, (std::array<std::uint8_t, 1>{ 0xC9 }));
DEFINE_PATCH(FixHolidayConsumables, 0x140B3F615, (std::array<std::uint8_t, 6>{ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }));
} // namespace gw1

namespace gw2
{
//--- PvZ: GW2
DEFINE_INSTANCE(AppId, 0x1421A0A28);
DEFINE_INSTANCE(EmptyStr, 0x14294ED54);
DEFINE_INSTANCE(EmptyArray, 0x14294E450);
DEFINE_INSTANCE(SettingsManager, 0x142B508E0);

//--- Utility
DEFINE_FUNCTION(GetOptionParameter, 0x1401C51E0);
DEFINE_FUNCTION(SetInclusionOptions, 0x14038DBB0);
DEFINE_FUNCTION(GetContainer, 0x1401F2360);

//--- Hooks
DEFINE_FUNCTION(InitSettings, 0x140131ED0);
DEFINE_FUNCTION(MainInit, 0x14012CBB0);
DEFINE_FUNCTION(ServerStart, 0x1405D5FE0);
DEFINE_FUNCTION(CreateServer, 0x14012E880);
DEFINE_FUNCTION(ClientInitNetwork, 0x140444B50);
DEFINE_FUNCTION(ClientConnectToAddress, 0x14054ABE0);
DEFINE_FUNCTION(NetworkEnginePeerInit, 0x14070CF70);
DEFINE_FUNCTION(GetPlayerName, 0x1401F7C00);
DEFINE_FUNCTION(ServerLoadLevelMessagePost, 0x1405D85E0);
DEFINE_FUNCTION(MessageManagerExecuteMessage, 0x1401A1170);
DEFINE_FUNCTION(getPlatformDataType, 0x1401758D0);
DEFINE_FUNCTION(getBackendType, 0x140EAA8C0);

DEFINE_PATCH(AllowMultipleInstances, 0x14013A4C6, (std::array<std::uint8_t, 1>{ 0xEB }));
DEFINE_PATCH(MemFreeBug, 0x140155F01, (std::array<std::uint8_t, 1>{ 0xEB }));
DEFINE_PATCH(SkipStreamInstallInstallation, 0x14012CE0C, (std::array<std::uint8_t, 3>{ 0x90, 0x90, 0x90 }));
DEFINE_PATCH(AllowCommandArgumentsAsOptions, 0x1401328DA, (std::array<std::uint8_t, 1>{ 0xE6 }));
DEFINE_PATCH(PlayerNameFormatting, 0x1422DC782, (std::array<std::uint8_t, 3>{ 0x00, 0x00, 0x00 }));
DEFINE_PATCH(InfiniteConsumables, 0x140d8ef3e, (std::array<std::uint8_t, 6>{ 0xBA, 0x41, 0x00, 0x00, 0x00, 0x90 }));
} // namespace gw2

namespace gw3
{
//--- PvZ: BFN
DEFINE_INSTANCE(AppId, 0x14321EB58);
DEFINE_INSTANCE(SettingsManager, 0x14421BFD0);
DEFINE_INSTANCE(PVZOnlineSettings, 0x144CACF70);
DEFINE_INSTANCE(PVZServerSettings, 0x144DD8E30);
DEFINE_INSTANCE(GameModeSettings, 0x144E101D0);
DEFINE_INSTANCE(NetObjectSettings, 0x144C80250);
DEFINE_INSTANCE(GameSettings, 0x144C257F0);

//--- Utility
DEFINE_FUNCTION(GetOptionParameter, 0x1404681C0);
DEFINE_FUNCTION(GetSettings, 0x14046F230);
DEFINE_FUNCTION(fbString, 0x14041B6C0);
DEFINE_FUNCTION(LevelSetupInit, 0x142BB5CF0);
DEFINE_FUNCTION(SetInclusionOptions, 0x140F9DD60);

//--- Hooks
DEFINE_FUNCTION(UserAdded, 0x1417A3910);
DEFINE_FUNCTION(ServerStart, 0x140F23240);
DEFINE_FUNCTION(SpawnServer, 0x14030AAD0);
DEFINE_FUNCTION(ClientInitNetwork, 0x140F0ACE0);
DEFINE_FUNCTION(ClientConnectToAddress, 0x140F3B480);
DEFINE_FUNCTION(NetworkEnginePeerInit, 0x1410FDE90);
DEFINE_FUNCTION(OnEvent, 0x14155DF60);
DEFINE_FUNCTION(PeerHasJoined, 0x140F55A00);
DEFINE_FUNCTION(GetPlayerName, 0x14155DCA0);
DEFINE_FUNCTION(CreateUser, 0x1426171F0);
DEFINE_FUNCTION(MessageManagerDispatchMessage, 0x14046E990);
DEFINE_FUNCTION(ServerLoadLevelPost, 0x140F4D5D0);
DEFINE_FUNCTION(GameSimulationInit, 0x1403090C0);
DEFINE_FUNCTION(InitDataPlatform, 0x142216DF0);
DEFINE_FUNCTION(CreateGameWindow, 0x14058F890);

DEFINE_PATCH(NoWindow, 0x14058F8DB, (std::array<std::uint8_t, 3>{ 0x90, 0x90, 0x90 }));
DEFINE_PATCH(MemFreeBug, 0x140F21C43, (std::array<std::uint8_t, 1>{ 0xEB }));
DEFINE_PATCH(MultipleInstances, 0x142219D26, (std::array<std::uint8_t, 1>{ 0xEB }));

DEFINE_PATCH(AllowCommandArgumentsAsOptions, 0x142218419, (std::array<std::uint8_t, 1>{ 0xA7 }));
DEFINE_PATCH(IgnoreEasyAntiCheat, 0x1417fd2d8, (std::array<std::uint8_t, 1>{ 0xEB }));
DEFINE_PATCH(OfflineFix, 0x1415704D0, (std::array<std::uint8_t, 3>{ 0xB0, 0x01, 0xC3 }));
DEFINE_PATCH(Killswitches, 0x1417288A9, (std::array<std::uint8_t, 3>{ 0xE9, 0xBA, 0x00 }));
DEFINE_PATCH(UnlockAwards, 0x141E9BD70, (std::array<std::uint8_t, 4>{ 0x48, 0x31, 0xC0, 0xC3 }));
} // namespace gw3

template<class T>
[[nodiscard]] inline T ptr(std::uintptr_t a) noexcept
{
    return reinterpret_cast<T>(a);
}

template<class T>
[[nodiscard]] inline T ptr(void* p) noexcept
{
    return reinterpret_cast<T>(p);
}
} // namespace offsets
