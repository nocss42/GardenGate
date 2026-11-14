"""Garden Warfare/2/BFN for EA App"""

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

    util.replace_command('steam2ea', 'placeholder') # causes game to not launch via steam2ea
