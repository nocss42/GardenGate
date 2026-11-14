"""Garden Warfare 2"""

from protonfixes import util

def early() -> None:
    util.set_environment(
        'PROTON_REMOTE_DEBUG_CMD',
        '/mnt/Games/SteamGames/steamapps/common/PVZGW2/GardenGate/GardenGate_Launcher.exe',
    )

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
