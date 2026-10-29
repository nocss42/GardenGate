#!/usr/bin/env python3

import os
import sys
import subprocess

HOME = os.path.expanduser('~')

# Non steam variables
SLR_PATH = os.environ.get('SLR_PATH', os.path.join(HOME, '.local/share/umu/steamrt4')) # Custom for us
PROTONPATH = os.path.join(HOME, '.local/share/Steam/compatibilitytools.d/GE-Proton11-11-x86_64') # based on UMU

# Custom configuration
WINE_EXECUTABLE = sys.argv[1]
args = sys.argv[2:]
EXE_PATH = "".join(WINE_EXECUTABLE.split("/")[:-1]) # In case install path is not set
PROTON_VERB = os.environ.get('PROTON_VERB', 'run') # Proton verb (run, waitforexitandrun, etc.)

# Create wine prefix if it doesn't exist
#os.makedirs(os.environ.get('WINEPREFIX'), exist_ok=True)

# Execute the command with all necessary environment variables
env = os.environ.copy()

env.setdefault('STEAM_COMPAT_DATA_PATH', os.environ.get('WINEPREFIX')) # tried
env.setdefault('STEAM_COMPAT_CLIENT_INSTALL_PATH', os.path.join(HOME, '.steam/steam'))
#env.setdefault('STEAM_COMPAT_INSTALL_PATH', EXE_PATH) # Game install path, not steam
#env.setdefault('SteamAppId', '0')
#env.setdefault('STEAM_COMPAT_APP_ID', '0')
#env.setdefault('SteamGameId', '0')

cmd = [
    os.path.join(SLR_PATH, '_v2-entry-point'),
    '--',
    os.path.join(PROTONPATH, 'proton'),
    PROTON_VERB,
    WINE_EXECUTABLE,
] + args

subprocess.run(cmd, env=env)
