"""GardenGate https://github.com/nocss42/GardenGate"""

from protonfixes import util

def main() -> None:
    util.protontricks('vcrun2022')

    for dll in ('dinput8', 'winmm'):
        util.regedit_add(
            'HKEY_CURRENT_USER\\Software\\Wine\\AppDefaults\\PVZ.Main_Win64_Retail.exe\\DllOverrides',
            dll,
            'REG_SZ',
            'native,builtin',
        )

        util.regedit_add(
            'HKEY_CURRENT_USER\\Software\\Wine\\AppDefaults\\GW2.Main_Win64_Retail.exe\\DllOverrides',
            dll,
            'REG_SZ',
            'native,builtin',
        )

        util.regedit_add(
            'HKEY_CURRENT_USER\\Software\\Wine\\AppDefaults\\PVZBattleforNeighborville.exe\\DllOverrides',
            dll,
            'REG_SZ',
            'native,builtin',
        )
