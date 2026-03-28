# Frequently Asked Questions (FAQ)

### Are these official?
No, this is a private server project created for educational and experimental purposes. It is not affiliated with or endorsed by EA.

### Does this make gw2/bfn work with wine again?
Yes it should. Please [see this](./LINUX.md)

### Does this have some sort of anti-cheat of its own?
No and there aren't plans on implementing one anytime soon.

### Does this allow illegal copies of the game to be played online?
It doesn't alter the drm in any way. It's highly recommended that you don't use illegal copies of the game.

### Does this work on console?
No it doesn't.

### Does this "steal" my ip?
No it doesn't. It's just shared just like minecraft servers.

### Does progression work on this?
No it doesn't. There aren't plans on fixing that as of writing this.

### I'm having issue X is this known?
Check if it's been added to the what's not done part of [README](../README.md)

### Do I need to install the recommended mods for gw2? Do I need every mod the server I'm joining has?
For gw2 (only one that needs mods) just the host needs to install the mods. For small client sided mods like FOV changer not really, but big mods like project visionary will make it crash if you don't have them installed.

### Is there a way to host this as a dedicated server on linux/windows?
Yes. For linux [see this](./LINUX_DEDICATED.md) . If you'd like to run a server on windows just use wsl to run podman or just setup the game how you would normally (installing ea app/maxima & frosty) and use -server launch arguement. 

### Who made the DLL for this?
@sonicrat did the whole thing.

### Why is there an AppImage for this?
The launcher uses tauri which uses webview2 which runs very poorly with wine. The appimage is for rcon only as there isn't an [umu](https://github.com/Open-Wine-Components/umu-launcher) implementation as of right now.

### How do I contribute?
1. Fork the repository.
2. Commit your changes.
3. Push to your fork.
4. Open a Pull Request.
