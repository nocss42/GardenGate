#include "Patcher.hpp"
#include "Utils.hpp"

#include "xdelta3_wrapper.h"
#include <windows.h>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

constexpr const char *BACKUP = ".bak";
constexpr const char *PATCHED = ".patched";
constexpr const char *DISABLED_DLL = ".disabled";

static bool run_xdelta(const std::string &original, const std::string &patch, const std::string &output, std::string &err)
{
    std::vector<std::string> params = {"-d", "-f", "-s", original, patch, output};
    int result = xd3_main_exec(params);
    std::string messages = xd3_messages();

    if (result != 0)
    {
        err = "xdelta3 failed: " + (messages.empty()
                                        ? "Unknown error (code: " + std::to_string(result) + ")"
                                        : messages);
        return false;
    }
    return true;
}

bool Patcher::IsGW2Patched(const std::string &gamePath)
{
    if (gamePath.empty() || !fs::exists(gamePath))
        return false;

    std::string backupExe = gamePath + BACKUP;
    if (!fs::exists(backupExe))
        return false;

    fs::path gameDir = fs::path(gamePath).parent_path();
    return fs::exists(gameDir / "dinput8.dll");
}

bool Patcher::AutoPatchGW2(const std::string &gamePath, const std::string &patchFile, const std::string &dllFile, std::string &errorMessage)
{
    if (!fs::exists(gamePath))
    {
        errorMessage = "Game executable not found";
        return false;
    }
    if (!fs::exists(patchFile))
    {
        errorMessage = "Patch file not found";
        return false;
    }
    if (!fs::exists(dllFile))
    {
        errorMessage = "dinput8.dll not found";
        return false;
    }

    std::string backupExe = gamePath + BACKUP;
    Utils::FileUtil::remove(backupExe);
    if (!Utils::FileUtil::copy(gamePath, backupExe, errorMessage))
        return false;

    fs::path dllTarget = fs::path(gamePath).parent_path() / "dinput8.dll";

    std::error_code ec;
    bool isSameFile = fs::exists(dllTarget) && fs::equivalent(dllFile, dllTarget, ec);

    if (!isSameFile)
    {
        if (!Utils::FileUtil::copy(dllFile, dllTarget.string(), errorMessage))
        {
            if (fs::exists(dllTarget))
            {
                std::string dllBackup = dllTarget.string() + ".old";
                Utils::FileUtil::move_replace(dllTarget.string(), dllBackup, errorMessage);
                Utils::FileUtil::copy(dllFile, dllTarget.string(), errorMessage);
            }
            else
                return false;
        }
    }

    std::string tempOutput = gamePath + PATCHED;
    if (!run_xdelta(gamePath, patchFile, tempOutput, errorMessage))
    {
        Utils::FileUtil::remove(backupExe);
        if (!isSameFile)
        {
            Utils::FileUtil::remove(dllTarget.string());
        }
        return false;
    }

    if (!Utils::FileUtil::move_replace(tempOutput, gamePath, errorMessage))
    {
        Utils::FileUtil::remove(backupExe);
        if (!isSameFile)
        {
            Utils::FileUtil::remove(dllTarget.string());
        }
        return false;
    }

    fs::path gameDir = fs::path(gamePath).parent_path();
    fs::path eaAntiCheat = gameDir / "EAAntiCheat.GameServiceLauncher.exe";
    if (fs::exists(eaAntiCheat))
    {
        std::string eaDisabled = eaAntiCheat.string() + DISABLED_DLL;
        if (fs::exists(eaDisabled))
            Utils::FileUtil::remove(eaDisabled);
        Utils::FileUtil::move_replace(eaAntiCheat.string(), eaDisabled, errorMessage);
    }

    fs::path installScript = gameDir / "installScript.vdf";
    if (fs::exists(installScript))
    {
        std::string scriptDisabled = installScript.string() + DISABLED_DLL;
        if (fs::exists(scriptDisabled))
            Utils::FileUtil::remove(scriptDisabled);
        Utils::FileUtil::move_replace(installScript.string(), scriptDisabled, errorMessage);
    }

    return true;
}

bool Patcher::RestoreGW2(const std::string &gamePath, std::string &errorMessage)
{
    if (!fs::exists(gamePath))
    {
        errorMessage = "Game executable not found";
        return false;
    }

    std::string backupExe = gamePath + BACKUP;
    if (!fs::exists(backupExe))
    {
        errorMessage = "Backup EXE not found";
        return false;
    }

    if (!Utils::FileUtil::copy(backupExe, gamePath, errorMessage))
        return false;

    fs::path gameDir = fs::path(gamePath).parent_path();
    fs::path dllPath = gameDir / "dinput8.dll";
    if (fs::exists(dllPath))
    {
        Utils::FileUtil::remove(dllPath.string());
    }

    fs::path eaAntiCheat = gameDir / "EAAntiCheat.GameServiceLauncher.exe";
    std::string eaDisabled = eaAntiCheat.string() + DISABLED_DLL;
    if (fs::exists(eaDisabled))
    {
        Utils::FileUtil::move_replace(eaDisabled, eaAntiCheat.string(), errorMessage);
    }

    fs::path installScript = gameDir / "installScript.vdf";
    std::string scriptDisabled = installScript.string() + DISABLED_DLL;
    if (fs::exists(scriptDisabled))
    {
        Utils::FileUtil::move_replace(scriptDisabled, installScript.string(), errorMessage);
    }

    Utils::FileUtil::remove(backupExe);
    return true;
}

bool Patcher::IsBFNPatched(const std::string &gamePath)
{
    if (gamePath.empty() || !fs::exists(gamePath))
        return false;

    std::string backupExe = gamePath + BACKUP;
    if (!fs::exists(backupExe))
        return false;

    fs::path gameDir = fs::path(gamePath).parent_path();
    return fs::exists(gameDir / "dinput8.dll");
}

bool Patcher::AutoPatchBFN(const std::string &gamePath, const std::string &patchesDir, const std::string &dllFile, std::string &errorMessage)
{
    if (!fs::exists(gamePath))
    {
        errorMessage = "Game executable not found";
        return false;
    }
    if (!fs::exists(patchesDir))
    {
        errorMessage = "Patches directory not found";
        return false;
    }
    if (!fs::exists(dllFile))
    {
        errorMessage = "dinput8.dll not found";
        return false;
    }

    fs::path gameDir = fs::path(gamePath).parent_path();

    std::string backupExe = gamePath + BACKUP;
    Utils::FileUtil::remove(backupExe);
    if (!Utils::FileUtil::copy(gamePath, backupExe, errorMessage))
        return false;

    fs::path dllTarget = gameDir / "dinput8.dll";

    std::error_code ec;
    bool isSameFile = fs::exists(dllTarget) && fs::equivalent(dllFile, dllTarget, ec);

    if (!isSameFile)
    {
        if (!Utils::FileUtil::copy(dllFile, dllTarget.string(), errorMessage))
        {
            if (fs::exists(dllTarget))
            {
                std::string dllBackup = dllTarget.string() + ".old";
                Utils::FileUtil::move_replace(dllTarget.string(), dllBackup, errorMessage);
                Utils::FileUtil::copy(dllFile, dllTarget.string(), errorMessage);
            }
            else
            {
                Utils::FileUtil::remove(backupExe);
                return false;
            }
        }
    }

    std::string exePatchFile = (fs::path(patchesDir) / "PVZBattleforNeighborville.exe.xdelta").string();
    if (!fs::exists(exePatchFile))
    {
        errorMessage = "Missing PVZBattleforNeighborville.exe.xdelta";
        return false;
    }

    std::string tempOutput = gamePath + PATCHED;
    if (!run_xdelta(gamePath, exePatchFile, tempOutput, errorMessage))
    {
        Utils::FileUtil::remove(backupExe);
        if (!isSameFile)
        {
            Utils::FileUtil::remove(dllTarget.string());
        }
        return false;
    }

    if (!Utils::FileUtil::move_replace(tempOutput, gamePath, errorMessage))
    {
        Utils::FileUtil::remove(backupExe);
        if (!isSameFile)
        {
            Utils::FileUtil::remove(dllTarget.string());
        }
        return false;
    }

    fs::path activationDll = gameDir / "Core" / "Activation.dll";
    if (fs::exists(activationDll))
    {
        std::string activationPatch = (fs::path(patchesDir) / "Core" / "Activation.dll.xdelta").string();
        if (fs::exists(activationPatch))
        {
            std::string backupDll = activationDll.string() + BACKUP;
            Utils::FileUtil::copy(activationDll.string(), backupDll, errorMessage);

            std::string tempDll = activationDll.string() + PATCHED;
            if (run_xdelta(activationDll.string(), activationPatch, tempDll, errorMessage))
            {
                Utils::FileUtil::move_replace(tempDll, activationDll.string(), errorMessage);
            }
        }
    }

    fs::path activation64Dll = gameDir / "Core" / "Activation64.dll";
    if (fs::exists(activation64Dll))
    {
        std::string activation64Patch = (fs::path(patchesDir) / "Core" / "Activation64.dll.xdelta").string();
        if (fs::exists(activation64Patch))
        {
            std::string backupDll = activation64Dll.string() + BACKUP;
            Utils::FileUtil::copy(activation64Dll.string(), backupDll, errorMessage);

            std::string tempDll = activation64Dll.string() + PATCHED;
            if (run_xdelta(activation64Dll.string(), activation64Patch, tempDll, errorMessage))
            {
                Utils::FileUtil::move_replace(tempDll, activation64Dll.string(), errorMessage);
            }
        }
    }

    fs::path installerDir = gameDir / "__Installer";
    if (fs::exists(installerDir))
    {
        std::vector<std::string> installerFiles = {
            "Cleanup.dat", "Cleanup.exe", "Touchup.dat", "Touchup.exe", "installerdata.xml"};

        for (const auto &filename : installerFiles)
        {
            fs::path targetFile = installerDir / filename;
            if (fs::exists(targetFile))
            {
                std::string patchFile = (fs::path(patchesDir) / "__Installer" / (filename + ".xdelta")).string();
                if (fs::exists(patchFile))
                {
                    std::string backupFile = targetFile.string() + BACKUP;
                    Utils::FileUtil::copy(targetFile.string(), backupFile, errorMessage);

                    std::string tempFile = targetFile.string() + PATCHED;
                    if (run_xdelta(targetFile.string(), patchFile, tempFile, errorMessage))
                    {
                        Utils::FileUtil::move_replace(tempFile, targetFile.string(), errorMessage);
                    }
                }
            }
        }
    }

    fs::path eaAntiCheat = gameDir / "EAAntiCheat.GameServiceLauncher.exe";
    if (fs::exists(eaAntiCheat))
    {
        std::string eaDisabled = eaAntiCheat.string() + DISABLED_DLL;
        if (fs::exists(eaDisabled))
            Utils::FileUtil::remove(eaDisabled);
        Utils::FileUtil::move_replace(eaAntiCheat.string(), eaDisabled, errorMessage);
    }

    fs::path installScript = gameDir / "installScript.vdf";
    if (fs::exists(installScript))
    {
        std::string scriptDisabled = installScript.string() + DISABLED_DLL;
        if (fs::exists(scriptDisabled))
            Utils::FileUtil::remove(scriptDisabled);
        Utils::FileUtil::move_replace(installScript.string(), scriptDisabled, errorMessage);
    }

    return true;
}

bool Patcher::RestoreBFN(const std::string &gamePath, std::string &errorMessage)
{
    if (!fs::exists(gamePath))
    {
        errorMessage = "Game executable not found";
        return false;
    }

    std::string backupExe = gamePath + BACKUP;
    if (!fs::exists(backupExe))
    {
        errorMessage = "Backup EXE not found";
        return false;
    }

    if (!Utils::FileUtil::copy(backupExe, gamePath, errorMessage))
        return false;

    fs::path gameDir = fs::path(gamePath).parent_path();

    fs::path dllPath = gameDir / "dinput8.dll";
    if (fs::exists(dllPath))
    {
        Utils::FileUtil::remove(dllPath.string());
    }

    fs::path activationDll = gameDir / "Core" / "Activation.dll";
    std::string activationBackup = activationDll.string() + BACKUP;
    if (fs::exists(activationBackup))
    {
        Utils::FileUtil::copy(activationBackup, activationDll.string(), errorMessage);
        Utils::FileUtil::remove(activationBackup);
    }

    fs::path activation64Dll = gameDir / "Core" / "Activation64.dll";
    std::string activation64Backup = activation64Dll.string() + BACKUP;
    if (fs::exists(activation64Backup))
    {
        Utils::FileUtil::copy(activation64Backup, activation64Dll.string(), errorMessage);
        Utils::FileUtil::remove(activation64Backup);
    }

    fs::path installerDir = gameDir / "__Installer";
    if (fs::exists(installerDir))
    {
        std::vector<std::string> installerFiles = {
            "Cleanup.dat", "Cleanup.exe", "Touchup.dat", "Touchup.exe", "installerdata.xml"};

        for (const auto &filename : installerFiles)
        {
            fs::path targetFile = installerDir / filename;
            std::string backupFile = targetFile.string() + BACKUP;
            if (fs::exists(backupFile))
            {
                Utils::FileUtil::copy(backupFile, targetFile.string(), errorMessage);
                Utils::FileUtil::remove(backupFile);
            }
        }
    }

    fs::path eaAntiCheat = gameDir / "EAAntiCheat.GameServiceLauncher.exe";
    std::string eaDisabled = eaAntiCheat.string() + DISABLED_DLL;
    if (fs::exists(eaDisabled))
    {
        Utils::FileUtil::move_replace(eaDisabled, eaAntiCheat.string(), errorMessage);
    }

    fs::path installScript = gameDir / "installScript.vdf";
    std::string scriptDisabled = installScript.string() + DISABLED_DLL;
    if (fs::exists(scriptDisabled))
    {
        Utils::FileUtil::move_replace(scriptDisabled, installScript.string(), errorMessage);
    }

    Utils::FileUtil::remove(backupExe);
    return true;
}

bool Patcher::ApplyPatch(const std::string &originalFile, const std::string &patchFile, const std::string &outputFile, std::string &errorMessage)
{
    if (!fs::exists(originalFile))
    {
        errorMessage = "Original file not found";
        return false;
    }
    if (!fs::exists(patchFile))
    {
        errorMessage = "Patch file not found";
        return false;
    }

    return run_xdelta(originalFile, patchFile, outputFile, errorMessage);
}