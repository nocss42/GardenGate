> [!WARNING]
> This doesn't work yet, very WIP.

------
This is simply adding GW1 (potentially GW2) dedicated server support.

A hosted version of Kyber is available at [kyber.gg](https://kyber.gg), where I operate proxies for server data that alleviate the issue of port forwarding and IP security.

What's done:
* Server starting
* Networking
* Direct proxy support
* NAT Punch-Through system
* In-Game server browser
* Per-player team swapping
* Player kicking/moderation
* Optimal proxy detection

What isn't done:
* Built-in mod verification (currently handled at the proxy level, meaning it's unavailable if you direct-connect)
* Kick messages (currently if you are kicked by the server admin you just get sent back to the menu with no indication of why)
* UX/UI Styling (WIP)
* Player banning
* Database handling at the proxy level

If you want to use Kyber purely without a proxy (port forwarding and having people connect to your IP), the rewrite (this) is completely usable for that, feel free to build Kyber and inject it with the new launcher (/Launcher, will need a few modifications to run your own dll).

**Stars and PRs are welcome!**

## Credits

Kyber utilizes the following open-source projects:

- [MinHook](https://github.com/TsudaKageyu/minhook)
- [ImGUI](https://github.com/ocornut/imgui)
- [GLFW](https://glfw.org)
- [cpp-httplib](https://github.com/yhirose/cpp-httplib)
- [openssl](https://openssl.org)
- [executors](https://github.com/chriskohlhoff/executors)
- [nlohmann-json](https://github.com/nlohmann/json)
