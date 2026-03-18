<div align="center">
  <img src="./Assets/logo.webp" alt="GardenGate" style="width: 10rem;">
  <h1 style="font-size: 2.5rem; margin-top: 0.5rem;">GardenGate</h1>
</div>

Experimental private servers for Plants vs. Zombies Garden Warfare 1, 2 & Battle for Neighborville

<p align="center">
  <a href="https://discord.gg/sEs9NcUrF9">
    <img src="https://img.shields.io/discord/1425183662080659570?label=Discord&logo=discord">
  </a>
  <a href="https://matrix.to/#/#gardengate:matrix.org">
    <img src="https://img.shields.io/matrix/gardengate:matrix.org?label=Matrix&logo=matrix">
  </a>
</p>

## Roadmap
### What's done:
* Offsets
* Joining
* Hosting servers (through client)
* Dedicated server support
* RCON
* Playlists
* Unlockers that unlock every item

### What's not done (TODO):
* Fix enchanted gardens/graveyards to be planted (these spawn entirely randomly) (e.g. boney island)
* [Closing a client](./Assets/client.webp) through the console instead of the game tricks the server into believing you are still connected, so attempting to load into another level causes the entire server to softlock
* Fix Zombopolis last, several doorways in the center area are blocked for clients. Host and bots can pass through, but clients and their objects (e.g. bean bombs) cannot.
* Fix Zomburbia Zombot fuses on last point unable to be broken

## Directory structure
| Directory | Description                 |
|-----------|-----------------------------|
| `Assets` | Related assets               |
| `Container` | Container files           |
| `DLL` | DLL injected to the game client |
| `Docs` | Documentation                  |
| `Launcher` | Launcher application       |
| `Mods` | QoL mods                       |

## Credits

- sonicrat

- nocss

- twig

- objectiveswift

- blueballoon

- eshaydev

- gargos69junior

- werzdragon

- megadwarf47

- puro420

### Third-Party
Following open-source projects were used:

- [MinHook](https://github.com/TsudaKageyu/minhook)
- [Kyber](https://github.com/ArmchairDevelopers/Kyber)
- [ImGui](https://github.com/ocornut/imgui)
- [Spdlog](https://github.com/gabime/spdlog)
- [Json](https://github.com/nlohmann/json)
- [XDelta](https://github.com/marco-calautti/xdelta)
