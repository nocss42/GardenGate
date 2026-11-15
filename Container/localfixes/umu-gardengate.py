"""Garden Warfare/Garden Warfare 2/Battle for Neighborville for EA App"""

from protonfixes import util

def main() -> None:
    util.protontricks('vcrun2022')

    for dll in ('dinput8', 'winmm'):
        for exe in (
            'PVZ.Main_Win64_Retail.exe',
            'GW2.Main_Win64_Retail.exe',
            'PVZBattleforNeighborville.exe',
        ):
            util.regedit_add(
                f'HKEY_CURRENT_USER\\Software\\Wine\\AppDefaults\\{exe}\\DllOverrides',
                dll,
                'REG_SZ',
                'native,builtin',
            )
