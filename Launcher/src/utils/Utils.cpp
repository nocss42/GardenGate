#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <tlhelp32.h>
#include <process.h>

#include "Utils.hpp"
#include "Patcher.hpp"

#include <fstream>
#include <filesystem>
#include <map>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

namespace fs = std::filesystem;

namespace Utils
{
namespace Registry
{
bool IsSteamCopy(const std::string& exePath)
{
    if (exePath.empty())
        return false;

    std::string lowerPath = exePath;
    std::transform(lowerPath.begin(), lowerPath.end(), lowerPath.begin(), ::tolower);

    if (lowerPath.find("steamapps") != std::string::npos)
    {
        return true;
    }

    fs::path gameDir = fs::path(exePath).parent_path();
    if (fs::exists(gameDir / "steam_appid.txt"))
    {
        return true;
    }

    return false;
}

std::string GetGamePathFromRegistry(int gameType)
{
    HKEY hKey = NULL;
    LONG result = 0;
    char pathBuffer[MAX_PATH] = { 0 };
    DWORD bufferSize = MAX_PATH;

    std::string regPath;
    if (gameType == 1)
    {
        regPath = "SOFTWARE\\WOW6432Node\\PopCap\\Plants vs Zombies GW2";
        result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, regPath.c_str(), 0, KEY_READ, &hKey);
        if (result != ERROR_SUCCESS)
        {
            regPath = "SOFTWARE\\PopCap\\Plants vs Zombies GW2";
            result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, regPath.c_str(), 0, KEY_READ, &hKey);
        }
    }
    else if (gameType == 2)
    {
        regPath = "SOFTWARE\\WOW6432Node\\PopCap\\PVZ Battle for Neighborville";
        result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, regPath.c_str(), 0, KEY_READ, &hKey);
        if (result != ERROR_SUCCESS)
        {
            regPath = "SOFTWARE\\PopCap\\PVZ Battle for Neighborville";
            result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, regPath.c_str(), 0, KEY_READ, &hKey);
        }
    }
    else
    {
        regPath = "SOFTWARE\\WOW6432Node\\PopCap\\Plants vs Zombies Garden Warfare";
        result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, regPath.c_str(), 0, KEY_READ, &hKey);
        if (result != ERROR_SUCCESS)
        {
            regPath = "SOFTWARE\\PopCap\\Plants vs Zombies Garden Warfare";
            result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, regPath.c_str(), 0, KEY_READ, &hKey);
        }
    }

    if (result == ERROR_SUCCESS)
    {
        result = RegQueryValueExA(hKey, "Install Dir", NULL, NULL, (LPBYTE)pathBuffer, &bufferSize);
        if (result != ERROR_SUCCESS)
        {
            result = RegQueryValueExA(hKey, "Path", NULL, NULL, (LPBYTE)pathBuffer, &bufferSize);
        }
        RegCloseKey(hKey);
        return std::string(pathBuffer);
    }
    return "";
}
} // namespace Registry

namespace Dialog
{
std::string BrowseForExe(HWND hwnd, int gameType)
{
    char pathBuffer[MAX_PATH] = "";
    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;

    if (gameType == 1)
    {
        ofn.lpstrFilter = "GW2 Executable\0GW2.Main_Win64_Retail.exe;*.exe\0All Files\0*.*\0";
    }
    else if (gameType == 2)
    {
        ofn.lpstrFilter = "BFN Executable\0PVZBattleforNeighborville.exe;*.exe\0All Files\0*.*\0";
    }
    else
    {
        ofn.lpstrFilter = "GW1 Executable\0PVZ.Main_Win64_Retail.exe;*.exe\0All Files\0*.*\0";
    }

    ofn.lpstrFile = pathBuffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn))
        return pathBuffer;
    return "";
}
} // namespace Dialog

namespace UI
{
void CenteredInput(const char* label, std::string& value, float centerOffset, float fieldWidth, ImGuiInputTextFlags flags)
{
    ImGui::SetCursorPosX(centerOffset);
    ImGui::TextUnformatted(label);
    ImGui::SetCursorPosX(centerOffset);
    ImGui::SetNextItemWidth(fieldWidth);

    char buf[256];
    strncpy_s(buf, value.c_str(), sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    if (ImGui::InputText(std::string("##").append(label).c_str(), buf, sizeof(buf), flags))
    {
        value = buf;
        if (strcmp(label, "Username:") == 0 && value.length() > 30)
            value.resize(30);
    }
}

void CenteredText(const std::string& text, float windowWidth)
{
    float textWidth = ImGui::CalcTextSize(text.c_str()).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::TextUnformatted(text.c_str());
}

bool CenteredButton(const char* label, float width, float dpiScale)
{
    float safeWidth = ImGui::GetContentRegionAvail().x;
    float offset = (safeWidth - width) / 2.0f;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
    return ImGui::Button(label, ImVec2(width, 38.0f * dpiScale));
}

namespace Status
{
static StatusMessage currentStatus;

void Show(const std::string& text, bool isError, float duration)
{
    currentStatus = StatusMessage(text, isError, duration);
}

const StatusMessage& GetCurrent()
{
    return currentStatus;
}
} // namespace Status
} // namespace UI

namespace Process
{
static std::string UrlEncode(const std::string& input)
{
    std::ostringstream out;
    out << std::uppercase << std::hex;
    for (unsigned char c : input)
    {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~')
        {
            out << c;
        }
        else
        {
            out << '%' << std::setw(2) << std::setfill('0') << (int)c;
        }
    }
    return out.str();
}

static std::string NormalizeDataPath(const std::string& dataPath)
{
    if (dataPath.empty())
        return dataPath;
    return fs::path(dataPath).make_preferred().string();
}

std::string BuildArgs(const Config& cfg)
{
    const GameConfig& game = cfg.get_current_game();
    std::vector<std::string> argList;

    if (!game.custom_args.empty())
        argList.push_back(game.custom_args);

    if (!cfg.username.empty())
        argList.push_back("-name \"" + cfg.username + "\"");
    if (!cfg.server_ip.empty())
        argList.push_back("-Client.ServerIp " + cfg.server_ip);
    if (!cfg.password.empty())
        argList.push_back("-Server.ServerPassword " + cfg.password);

    std::string args;
    for (size_t i = 0; i < argList.size(); ++i)
    {
        if (i)
            args += ' ';
        args += argList[i];
    }

    return args;
}

static std::map<std::string, std::string> originalArgs;
static const std::string kMissingMarker = "__GG__MISSING__";

void PatchEAArgs(const std::string& args, int gameType)
{
    fs::path eaDir = fs::path(getenv("LOCALAPPDATA")) / "Electronic Arts" / "EA Desktop";

    if (!fs::exists(eaDir) || !fs::is_directory(eaDir))
    {
        return;
    }

    const std::vector<std::string> gw1Keys = { "user.gamecommandline.ofb-east:109551084", "user.gamecommandline.ofb-east:109550787",
        "user.gamecommandline.ofb-east:109552442" };
    const std::vector<std::string> gw2Keys = { "user.gamecommandline.origin.ofr.50.0001051", "user.gamecommandline.origin.ofr.50.0000786" };
    const std::vector<std::string> bfnKeys = { "user.gamecommandline.origin.ofr.50.0002623", "user.gamecommandline.origin.ofr.50.0003568",
        "user.gamecommandline.origin.ofr.50.0003634", "user.gamecommandline.origin.ofr.50.0003673" };

    const auto& keys = (gameType == 1) ? gw2Keys : (gameType == 2) ? bfnKeys : gw1Keys;

    std::string cleanedArgs = args;
    if (!cleanedArgs.empty())
    {
        cleanedArgs.erase(std::remove(cleanedArgs.begin(), cleanedArgs.end(), '\"'), cleanedArgs.end());
    }

    for (const auto& entry : fs::directory_iterator(eaDir))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".ini")
        {
            std::ifstream in(entry.path());
            if (!in)
                continue;

            std::string line, output;
            bool modified = false;
            std::map<std::string, bool> foundKeys;
            for (const auto& key : keys)
            {
                foundKeys[key] = false;
            }

            while (std::getline(in, line))
            {
                for (const auto& key : keys)
                {
                    if (line.rfind(key + "=", 0) == 0)
                    {
                        foundKeys[key] = true;
                        auto pos = line.find('=');
                        if (pos != std::string::npos)
                        {
                            std::string mapKey = entry.path().string() + "|" + key;
                            if (originalArgs.find(mapKey) == originalArgs.end())
                            {
                                originalArgs[mapKey] = line.substr(pos + 1);
                            }
                            line = line.substr(0, pos + 1) + cleanedArgs;
                            modified = true;
                        }
                        break;
                    }
                }
                output += line + "\n";
            }
            in.close();

            for (const auto& key : keys)
            {
                if (!foundKeys[key])
                {
                    std::string mapKey = entry.path().string() + "|" + key;
                    if (originalArgs.find(mapKey) == originalArgs.end())
                    {
                        originalArgs[mapKey] = kMissingMarker;
                    }
                    output += key + "=" + cleanedArgs + "\n";
                    modified = true;
                }
            }

            if (modified)
            {
                std::ofstream out(entry.path(), std::ios::trunc);
                out << output;
            }
        }
    }
}

void RestoreEAArgs()
{
    for (const auto& [filePath, originalValue] : originalArgs)
    {
        auto splitPos = filePath.rfind('|');
        if (splitPos == std::string::npos)
            continue;
        std::string path = filePath.substr(0, splitPos);
        std::string key = filePath.substr(splitPos + 1);

        if (!fs::exists(path))
        {
            continue;
        }

        std::ifstream in(path);
        if (!in)
            continue;

        std::string line, output;
        bool modified = false;

        while (std::getline(in, line))
        {
            if (line.rfind(key + "=", 0) == 0)
            {
                if (originalValue == kMissingMarker)
                {
                    modified = true;
                    continue;
                }
                auto pos = line.find('=');
                if (pos != std::string::npos)
                {
                    line = line.substr(0, pos + 1) + originalValue;
                    modified = true;
                }
            }
            output += line + "\n";
        }
        in.close();

        if (modified)
        {
            std::ofstream out(path, std::ios::trunc);
            out << output;
        }
    }
    originalArgs.clear();
}

bool InjectDLL(DWORD processId, const std::string& dllPath)
{
    HANDLE hProcess = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, processId);
    if (!hProcess)
        return false;

    char fullPath[MAX_PATH];
    if (!GetFullPathNameA(dllPath.c_str(), MAX_PATH, fullPath, nullptr))
    {
        CloseHandle(hProcess);
        return false;
    }

    SIZE_T bytes = strlen(fullPath) + 1;
    LPVOID remoteMem = VirtualAllocEx(hProcess, nullptr, bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMem)
    {
        CloseHandle(hProcess);
        return false;
    }

    if (!WriteProcessMemory(hProcess, remoteMem, fullPath, bytes, nullptr))
    {
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    LPTHREAD_START_ROUTINE loadLibAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!loadLibAddr)
    {
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, loadLibAddr, remoteMem, 0, nullptr);
    if (!hThread)
    {
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return true;
}

void KillProcessByName(const std::string& processName)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &pe32))
    {
        do
        {
            if (_stricmp(pe32.szExeFile, processName.c_str()) == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                if (hProcess)
                {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                }
            }
        } while (Process32Next(snapshot, &pe32));
    }

    CloseHandle(snapshot);
}

LaunchResult LaunchAndInject(const std::string& exePath, const std::string& args, const std::string& dllName,
    const std::string& modDataPath, int gameType, const Config& cfg)
{
    KillProcessByName("EADesktop.exe");
    KillProcessByName("Origin.exe");
    LaunchResult lr;
    const std::string normalizedModDataPath = NormalizeDataPath(modDataPath);
    auto appendDataPathArg = [&](std::string& argStr) {
        if (normalizedModDataPath.empty())
            return;
        if (!argStr.empty())
            argStr += " ";
        argStr += "-dataPath " + normalizedModDataPath;
    };

    fs::path gameDir = fs::path(exePath).parent_path();

    bool hasSteamAppId = fs::exists(gameDir / "steam_appid.txt");
    bool hasInstaller = fs::exists(gameDir / "__Installer");
    bool launchDirectly = cfg.launch_directly;

    if (launchDirectly || (!hasSteamAppId && !hasInstaller))
    {
        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi{};

        std::string effectiveArgs = args;
        if (gameType > 0)
        {
            appendDataPathArg(effectiveArgs);
        }

        std::string cmdLineStr = "\"" + exePath + "\" " + effectiveArgs;
        std::vector<char> cmdLine(cmdLineStr.begin(), cmdLineStr.end());
        cmdLine.push_back('\0');

        LPVOID envBlock = nullptr;
        if (!normalizedModDataPath.empty() && gameType == 0)
        {
            std::string envVarDir = "GAME_DATA_DIR=" + normalizedModDataPath;
            std::string envStr;

            LPCH envStrings = GetEnvironmentStringsA();
            if (envStrings)
            {
                LPCH p = envStrings;
                while (*p)
                {
                    envStr += p;
                    envStr += '\0';
                    p += strlen(p) + 1;
                }
                FreeEnvironmentStringsA(envStrings);
            }

            envStr += envVarDir;
            envStr += '\0';
            envStr += '\0';

            std::vector<char> envBuffer(envStr.begin(), envStr.end());
            envBlock = envBuffer.data();

            BOOL ok = CreateProcessA(nullptr, cmdLine.data(), nullptr, nullptr, FALSE, 0, envBlock, gameDir.string().c_str(), &si, &pi);

            if (!ok)
            {
                lr.error = "CreateProcess failed. Error: " + std::to_string(GetLastError());
                return lr;
            }
        }
        else
        {
            BOOL ok = CreateProcessA(nullptr, cmdLine.data(), nullptr, nullptr, FALSE, 0, nullptr, gameDir.string().c_str(), &si, &pi);

            if (!ok)
            {
                lr.error = "CreateProcess failed. Error: " + std::to_string(GetLastError());
                return lr;
            }
        }

        lr.ok = true;
        lr.pi = pi;

        if (gameType == 0)
        {
            fs::path dllPath = gameDir / dllName;
            if (!fs::exists(dllPath))
            {
                lr.error = dllName + " not found next to the game exe.";
                lr.ok = false;
                return lr;
            }

            const wchar_t* Gw1Title = L"PVZ Garden Warfare";
            DWORD pid = 0;
            int waited = 0;
            while (waited < 30000)
            {
                HWND hwnd = FindWindowW(nullptr, Gw1Title);
                if (hwnd)
                {
                    GetWindowThreadProcessId(hwnd, &pid);
                    if (pid)
                        break;
                }
                Sleep(500);
                waited += 500;
            }

            if (!pid)
            {
                lr.error = "Timed out waiting for GW1 window.";
                lr.ok = false;
                return lr;
            }

            Sleep(4000);

            if (!InjectDLL(pid, dllPath.string()))
            {
                lr.error = "DLL injection into GW1 failed.";
                lr.ok = false;
                return lr;
            }
        }

        return lr;
    }

    bool isSteamCopy = (gameType == 1 || gameType == 2) && hasSteamAppId;
    if (isSteamCopy)
    {
        KillProcessByName("steam.exe");
        std::string launchArgs = args;
        appendDataPathArg(launchArgs);

        std::string steamUrl;
        if (gameType == 2)
        {
            steamUrl = "steam://rungameid/1262240";
        }
        else
        {
            steamUrl = "steam://rungameid/1922560";
        }

        if (!launchArgs.empty())
        {
            steamUrl += "//" + UrlEncode(launchArgs);
        }

        HINSTANCE result = ShellExecuteA(nullptr, "open", steamUrl.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        if ((INT_PTR)result <= 32)
        {
            lr.error = "Failed to launch via Steam. Error code: " + std::to_string((INT_PTR)result);
            return lr;
        }

        lr.ok = true;
        return lr;
    }

    bool isOriginCopy = (gameType >= 1) && hasInstaller && !hasSteamAppId;
    if (isOriginCopy)
    {
        std::string launchArgs = args;
        appendDataPathArg(launchArgs);
        PatchEAArgs(launchArgs, gameType);

        std::string originUrl;
        if (gameType == 2)
        {
            originUrl = "origin://launchgame/194814";
        }
        else
        {
            originUrl = "origin://launchgame/1026482";
        }

        HINSTANCE result = ShellExecuteA(nullptr, "open", originUrl.c_str(), nullptr, nullptr, SW_SHOWNORMAL);

        if ((INT_PTR)result <= 32)
        {
            lr.error = "Failed to launch via Origin. Error code: " + std::to_string((INT_PTR)result);
            return lr;
        }

        lr.ok = true;
        return lr;
    }

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi{};

    std::string effectiveArgs = args;
    if (gameType > 0)
    {
        appendDataPathArg(effectiveArgs);
    }

    std::string cmdLineStr = "\"" + exePath + "\" " + effectiveArgs;
    std::vector<char> cmdLine(cmdLineStr.begin(), cmdLineStr.end());
    cmdLine.push_back('\0');

    LPVOID envBlock = nullptr;
    if (!normalizedModDataPath.empty() && gameType == 0)
    {
        std::string envVarDir = "GAME_DATA_DIR=" + normalizedModDataPath;
        std::string envStr;

        LPCH envStrings = GetEnvironmentStringsA();
        if (envStrings)
        {
            LPCH p = envStrings;
            while (*p)
            {
                envStr += p;
                envStr += '\0';
                p += strlen(p) + 1;
            }
            FreeEnvironmentStringsA(envStrings);
        }

        envStr += envVarDir;
        envStr += '\0';
        envStr += '\0';

        std::vector<char> envBuffer(envStr.begin(), envStr.end());
        envBlock = envBuffer.data();

        BOOL ok = CreateProcessA(
            nullptr, cmdLine.data(), nullptr, nullptr, FALSE, 0, envBlock, fs::path(exePath).parent_path().string().c_str(), &si, &pi);

        if (!ok)
        {
            lr.error = "CreateProcess failed. Error: " + std::to_string(GetLastError());
            return lr;
        }
    }
    else
    {
        BOOL ok = CreateProcessA(
            nullptr, cmdLine.data(), nullptr, nullptr, FALSE, 0, nullptr, fs::path(exePath).parent_path().string().c_str(), &si, &pi);

        if (!ok)
        {
            lr.error = "CreateProcess failed. Error: " + std::to_string(GetLastError());
            return lr;
        }
    }

    lr.ok = true;
    lr.pi = pi;

    if (gameType == 0)
    {
        fs::path dllPath = fs::path(exePath).parent_path() / dllName;
        if (!fs::exists(dllPath))
        {
            lr.error = dllName + " not found next to the game exe.";
            lr.ok = false;
            return lr;
        }

        const wchar_t* Gw1Title = L"PVZ Garden Warfare";
        DWORD pid = 0;
        int waited = 0;
        while (waited < 30000)
        {
            HWND hwnd = FindWindowW(nullptr, Gw1Title);
            if (hwnd)
            {
                GetWindowThreadProcessId(hwnd, &pid);
                if (pid)
                    break;
            }
            Sleep(500);
            waited += 500;
        }

        if (!pid)
        {
            lr.error = "Timed out waiting for GW1 window.";
            lr.ok = false;
            return lr;
        }

        Sleep(4000);

        if (!InjectDLL(pid, dllPath.string()))
        {
            lr.error = "DLL injection into GW1 failed.";
            lr.ok = false;
            return lr;
        }
    }

    return lr;
}

static std::vector<std::string> GetCommandLineArgs()
{
    int argc = 0;
    LPWSTR* argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
    std::vector<std::string> args;

    if (argvW)
    {
        for (int i = 0; i < argc; ++i)
        {
            int size = WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, nullptr, 0, nullptr, nullptr);
            std::string arg(size - 1, '\0');
            WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, &arg[0], size, nullptr, nullptr);
            args.push_back(arg);
        }
        LocalFree(argvW);
    }
    return args;
}

bool handleCLI()
{
    auto args = GetCommandLineArgs();
    if (args.size() < 3 || args[1] != "-patch")
        return false;

    std::ofstream log("gg_launcher.log", std::ios::app);
    auto logMsg = [&](const std::string& msg) { log << msg << std::endl; };

    auto runPatchTask = [&](const std::string& id, const std::string& dir) -> bool {
        if (dir.empty())
        {
            logMsg("No path provided or configured for " + id);
            return false;
        }

        std::string exeName, patchPath;
        if (id == "gw1")
            exeName = "PVZ.Main_Win64_Retail.exe";
        else if (id == "gw2")
        {
            exeName = "GW2.Main_Win64_Retail.exe";
            patchPath = "gw2/GW2.Main_Win64_Retail.exe.xdelta";
        }
        else if (id == "bfn")
        {
            exeName = "PVZBattleforNeighborville.exe";
            patchPath = "bfn";
        }

        char pathBuf[MAX_PATH];
        GetModuleFileNameA(NULL, pathBuf, MAX_PATH);
        fs::path launcherDir = fs::path(pathBuf).parent_path();
        fs::path dllSource = launcherDir / "dinput8.dll";
        fs::path fullExePath = fs::path(dir) / exeName;
        fs::path fullPatchPath = patchPath.empty() ? "" : (launcherDir / "patches" / patchPath);

        if (!fs::exists(fullExePath))
        {
            logMsg("FAILED: " + exeName + " not found at " + dir);
            return false;
        }

        logMsg("Patching " + id + "...");
        std::string err;
        bool ok = false;
        if (id == "gw1")
            ok = Patcher::AutoPatchGW1(fullExePath.string(), dllSource.string(), err);
        else if (id == "gw2")
            ok = Patcher::AutoPatchGW2(fullExePath.string(), fullPatchPath.string(), dllSource.string(), err);
        else if (id == "bfn")
            ok = Patcher::AutoPatchBFN(fullExePath.string(), fullPatchPath.string(), dllSource.string(), err);

        logMsg(ok ? "SUCCESS" : "FAILED: " + err);
        return ok;
    };

    Config cfg = load_config("config.json");
    bool overallSuccess = true;

    for (size_t i = 2; i < args.size(); ++i)
    {
        std::string arg = args[i];

        if (arg == "-all")
        {
            if (!runPatchTask("gw1", cfg.gw1.game_path))
                overallSuccess = false;
            if (!runPatchTask("gw2", cfg.gw2.game_path))
                overallSuccess = false;
            if (!runPatchTask("bfn", cfg.bfn.game_path))
                overallSuccess = false;
            continue;
        }

        if (arg == "gw1" || arg == "gw2" || arg == "bfn")
        {
            std::string path;
            if (i + 1 < args.size() && args[i + 1][0] != '-	')
            {
                path = args[i + 1];
                i++;
            }
            else
            {
                if (arg == "gw1")
                    path = cfg.gw1.game_path;
                else if (arg == "gw2")
                    path = cfg.gw2.game_path;
                else if (arg == "bfn")
                    path = cfg.bfn.game_path;
            }

            if (!runPatchTask(arg, path))
                overallSuccess = false;
        }
    }

    logMsg("========================================");
    return overallSuccess;
}
} // namespace Process

namespace FileUtil
{
bool copy(const std::string& src, const std::string& dst, std::string& err)
{
    if (!::CopyFileA(src.c_str(), dst.c_str(), FALSE))
    {
        err = "Failed to copy " + src + " to " + dst;
        return false;
    }
    return true;
}

bool move(const std::string& src, const std::string& dst, std::string& err)
{
    if (!::MoveFileA(src.c_str(), dst.c_str()))
    {
        err = "Failed to move " + src + " to " + dst;
        return false;
    }
    return true;
}

bool move_replace(const std::string& src, const std::string& dst, std::string& err)
{
    if (!::MoveFileExA(src.c_str(), dst.c_str(), MOVEFILE_REPLACE_EXISTING))
    {
        err = "Failed to replace " + dst + " with " + src;
        return false;
    }
    return true;
}

void remove(const std::string& path)
{
    ::DeleteFileA(path.c_str());
}
} // namespace FileUtil
} // namespace Utils
