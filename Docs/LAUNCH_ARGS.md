# Launch args

Here you can find launch arguments with their examples/defaults.

Please keep in mind some args here are tied to the `dinput8.dll`

### Universal
```
-name Player
-dataPath ModData/Default
-Client.ServerIp 127.0.0.1:25200
-listen 0.0.0.0:25200
-Server.ServerPassword 123456
-Window.Fullscreen true
-GameTime.MaxSimFps 60
```

### GW1
```
-level _pvz/Levels/Coastal/Level_COOP_Coastal/Level_COOP_Coastal
-Game.DefaultLayerInclusion GameMode=Coop0
-Network.ServerPort 25200
-PerfOverlay.DrawFps false
-PVZServer.InActivityTimeOut 180
-SyncedBFSettings.AllUnlocksUnlocked true
```

### GW2
```
-level Levels/Level_FE_Hub/Level_FE_Hub
-Game.DefaultLayerInclusion GameMode=FreeRoam
-Network.ServerPort 25200
-PerfOverlay.DrawFps false
-PVZServer.InActivityTimeOut 180
-GameMode.SkipIntroHubNIS true
-Online.OnlineGameInteractionMasterKillSwitch true
-Render.FovMultiplier 1.428571428571429
```

### BFN
```
-GameSettings.InitialDSubLevel Levels/DSub_SocialSpace/DSub_SocialSpace
-GameSettings.StartPoint StartPoint_SocialSpace
-GameSettings.DefaultLayerInclusion GameMode=Mode_SocialSpace;HostedMode=PeerHosted
-NetObjectSystem.MaxServerConnectionCount 4
-GameModeSettings.ForceHUBSeason 0
```

## Logging

### Console window

You can enable the console window by setting `GG_CONSOLE` environment variable to anything.

### Log level

You can switch between log levels by settings `GG_LOG_LEVEL` environment variable.

```
Debug++
Debug
Info
Warning
Error
Fatal
```

### Naming
You can edit the prefix of the .log file by setting `GG_LOG` environment variable. It'll appear like so when you do;

`GGLOGKEY.20380119_031407`
