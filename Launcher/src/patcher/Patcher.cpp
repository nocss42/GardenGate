#include "Patcher.hpp"
#include "Utils.hpp"
#include "xdelta3_wrapper.h"
#include <windows.h>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

constexpr const char* BACKUP = ".bak";
constexpr const char* PATCHED = ".patched";
constexpr const char* DISABLED_DLL = ".disabled";

namespace
{

bool run_xdelta(const std::string& original, const std::string& patch, const std::string& output, std::string& err)
{
    std::vector<std::string> params = { "-d", "-f", "-s", original, patch, output };
    int result = xd3_main_exec(params);
    if (result != 0)
    {
        std::string messages = xd3_messages();
        err = "xdelta3 failed: " + (messages.empty() ? "Unknown error (code: " + std::to_string(result) + ")" : messages);
        return false;
    }
    return true;
}

bool XdeltaPatch(const fs::path& target, const fs::path& patchFile, std::string& err, bool required = false)
{
    if (!fs::exists(target) || !fs::exists(patchFile))
    {
        if (required)
            err = "Required file missing for patching: " + target.filename().string();
        return !required; // If optional, missing file is "success" (skip)
    }

    std::string backupPath = target.string() + BACKUP;
    std::string tempPath = target.string() + PATCHED;

    if (!Utils::FileUtil::copy(target.string(), backupPath, err))
        return false;

    if (!run_xdelta(target.string(), patchFile.string(), tempPath, err))
    {
        Utils::FileUtil::remove(backupPath); // Rollback backup on fail
        return false;
    }

    if (!Utils::FileUtil::move_replace(tempPath, target.string(), err))
    {
        Utils::FileUtil::remove(backupPath);
        return false;
    }
    return true;
}

bool RestoreFromExtension(const fs::path& target, const std::string& ext, std::string& err)
{
    std::string backup = target.string() + ext;
    if (!fs::exists(backup))
        return true;

    if (fs::exists(target))
        Utils::FileUtil::remove(target.string());
    return Utils::FileUtil::move_replace(backup, target.string(), err);
}

void DisableFile(const fs::path& target, std::string& err)
{
    if (!fs::exists(target))
        return;
    std::string disabled = target.string() + DISABLED_DLL;
    if (fs::exists(disabled))
        Utils::FileUtil::remove(disabled);
    Utils::FileUtil::move_replace(target.string(), disabled, err);
}

void DisableEAServices(const fs::path& gameDir, std::string& err)
{
    DisableFile(gameDir / "EAAntiCheat.GameServiceLauncher.exe", err);
    DisableFile(gameDir / "installScript.vdf", err);
}

void RestoreEAServices(const fs::path& gameDir, std::string& err)
{
    RestoreFromExtension(gameDir / "EAAntiCheat.GameServiceLauncher.exe", DISABLED_DLL, err);
    RestoreFromExtension(gameDir / "installScript.vdf", DISABLED_DLL, err);
}

bool InjectDinput8(const fs::path& gameDir, const std::string& dllSource, std::string& err)
{
    fs::path dllTarget = gameDir / "dinput8.dll";

    std::error_code ec;
    if (fs::exists(dllTarget) && fs::equivalent(dllSource, dllTarget, ec))
    {
        return true;
    }

    if (fs::exists(dllTarget))
    {
        std::string dllBackup = dllTarget.string() + ".old";
        Utils::FileUtil::move_replace(dllTarget.string(), dllBackup, err);
    }

    return Utils::FileUtil::copy(dllSource, dllTarget.string(), err);
}

bool IsFrostbitePatched(const std::string& gamePath)
{
    if (gamePath.empty() || !fs::exists(gamePath))
        return false;
    if (!fs::exists(gamePath + BACKUP))
        return false;
    return fs::exists(fs::path(gamePath).parent_path() / "dinput8.dll");
}
} // namespace

bool Patcher::IsGW1Patched(const std::string& gamePath)
{
    if (gamePath.empty() || !fs::exists(gamePath))
        return false;
    return fs::exists(fs::path(gamePath).parent_path() / "dinput8.dll");
}

bool Patcher::IsGW2Patched(const std::string& gamePath)
{
    return IsFrostbitePatched(gamePath);
}
bool Patcher::IsBFNPatched(const std::string& gamePath)
{
    return IsFrostbitePatched(gamePath);
}

bool Patcher::AutoPatchGW1(const std::string& gamePath, const std::string& dllFile, std::string& errorMessage)
{
    if (!fs::exists(gamePath))
    {
        errorMessage = "Game executable not found";
        return false;
    }
    if (!fs::exists(dllFile))
    {
        errorMessage = "dinput8.dll not found";
        return false;
    }

    return InjectDinput8(fs::path(gamePath).parent_path(), dllFile, errorMessage);
}

bool Patcher::RestoreGW1(const std::string& gamePath, std::string& errorMessage)
{
    if (!fs::exists(gamePath))
    {
        errorMessage = "Game executable not found";
        return false;
    }

    fs::path dllPath = fs::path(gamePath).parent_path() / "dinput8.dll";
    return RestoreFromExtension(dllPath, BACKUP, errorMessage);
}

bool Patcher::AutoPatchGW2(const std::string& gamePath, const std::string& patchFile, const std::string& dllFile, std::string& errorMessage)
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

    fs::path gameDir = fs::path(gamePath).parent_path();

    if (!InjectDinput8(gameDir, dllFile, errorMessage))
        return false;
    if (!XdeltaPatch(gamePath, patchFile, errorMessage, true))
        return false;

    DisableEAServices(gameDir, errorMessage);
    return true;
}

bool Patcher::RestoreGW2(const std::string& gamePath, std::string& errorMessage)
{
    if (!fs::exists(gamePath))
    {
        errorMessage = "Game executable not found";
        return false;
    }

    fs::path gameDir = fs::path(gamePath).parent_path();

    if (!RestoreFromExtension(gamePath, BACKUP, errorMessage))
        return false;

    fs::path dllPath = gameDir / "dinput8.dll";
    if (fs::exists(dllPath))
        Utils::FileUtil::remove(dllPath.string());

    RestoreEAServices(gameDir, errorMessage);
    return true;
}

bool Patcher::AutoPatchBFN(
    const std::string& gamePath, const std::string& patchesDir, const std::string& dllFile, std::string& errorMessage)
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
    fs::path pDir = fs::path(patchesDir);

    if (!InjectDinput8(gameDir, dllFile, errorMessage))
        return false;

    if (!XdeltaPatch(gamePath, pDir / "PVZBattleforNeighborville.exe.xdelta", errorMessage, true))
        return false;

    XdeltaPatch(gameDir / "Core" / "Activation.dll", pDir / "Core" / "Activation.dll.xdelta", errorMessage, false);
    XdeltaPatch(gameDir / "Core" / "Activation64.dll", pDir / "Core" / "Activation64.dll.xdelta", errorMessage, false);

    std::vector<std::string> installerFiles = { "Cleanup.dat", "Cleanup.exe", "Touchup.dat", "Touchup.exe", "installerdata.xml" };
    for (const auto& filename : installerFiles)
    {
        XdeltaPatch(gameDir / "__Installer" / filename, pDir / "__Installer" / (filename + ".xdelta"), errorMessage, false);
    }

    DisableEAServices(gameDir, errorMessage);
    return true;
}

bool Patcher::RestoreBFN(const std::string& gamePath, std::string& errorMessage)
{
    if (!fs::exists(gamePath))
    {
        errorMessage = "Game executable not found";
        return false;
    }

    fs::path gameDir = fs::path(gamePath).parent_path();

    if (!RestoreFromExtension(gamePath, BACKUP, errorMessage))
        return false;

    fs::path dllPath = gameDir / "dinput8.dll";
    if (fs::exists(dllPath))
        Utils::FileUtil::remove(dllPath.string());

    RestoreFromExtension(gameDir / "Core" / "Activation.dll", BACKUP, errorMessage);
    RestoreFromExtension(gameDir / "Core" / "Activation64.dll", BACKUP, errorMessage);

    std::vector<std::string> installerFiles = { "Cleanup.dat", "Cleanup.exe", "Touchup.dat", "Touchup.exe", "installerdata.xml" };
    for (const auto& filename : installerFiles)
    {
        RestoreFromExtension(gameDir / "__Installer" / filename, BACKUP, errorMessage);
    }

    RestoreEAServices(gameDir, errorMessage);
    return true;
}

bool Patcher::ApplyPatch(
    const std::string& originalFile, const std::string& patchFile, const std::string& outputFile, std::string& errorMessage)
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