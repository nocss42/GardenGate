"""Battle for Neighborville"""

import os
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

    # Replace the Steam launch command with the GardenGate launcher itself.
    # WARNING
    # DO NOT NOP AND LAUNCH THE LAUNCHER LIKE SO IT'LL CAUSE BREAKAGE (any sort of steam2ea connection causing crashes)
    #util.set_environment('PROTON_REMOTE_DEBUG_CMD', os.path.abspath('GardenGate/GardenGate_Launcher.exe'), )
    #util.replace_command('steam2ea', 'placeholder') # block steam

    launcher = os.path.join(
        util.get_game_install_path(), 'GardenGate', 'GardenGate_Launcher.exe'
    )
    util.replace_command(r'steam2ea://\S+', launcher)
