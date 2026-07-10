#pragma once
#include <string>
#include <windows.h>
#include <imgui.h>
#include <vector>

#include "Config.hpp"

namespace Utils
{

namespace Registry
{
std::string GetGamePathFromRegistry(int gameType);
bool IsSteamCopy(const std::string& exePath);
} // namespace Registry

namespace Dialog
{
std::string BrowseForExe(HWND hwnd, int gameType);
}

namespace UI
{
void CenteredInput(const char* label, std::string& value, float centerOffset, float fieldWidth, ImGuiInputTextFlags flags = 0);
void CenteredText(const std::string& text, float windowWidth);
bool CenteredButton(const char* label, float width, float dpiScale);

namespace Status
{
struct StatusMessage
{
    std::string text;
    bool isError;
    std::chrono::steady_clock::time_point showTime;
    float duration;

    StatusMessage(const std::string& t = "", bool error = false, float dur = 3.0f)
        : text(t)
        , isError(error)
        , showTime(std::chrono::steady_clock::now())
        , duration(dur)
    {}

    bool isExpired() const
    {
        auto elapsed = std::chrono::steady_clock::now() - showTime;
        return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() > (duration * 1000.0f);
    }
};

void Show(const std::string& text, bool isError = false, float duration = 3.0f);
const StatusMessage& GetCurrent();
} // namespace Status
} // namespace UI

namespace Process
{
struct LaunchResult
{
    bool ok = false;
    std::string error;
    PROCESS_INFORMATION pi = { 0 };
};

std::string BuildArgs(const Config& cfg);
void PatchEAArgs(const std::string& args, int gameType);
void RestoreEAArgs();

bool handleCLI();

LaunchResult LaunchAndInject(const std::string& exePath, const std::string& args, const std::string& dllName,
    const std::string& modDataPath, int gameType, const Config& cfg);
} // namespace Process

namespace FileUtil
{
bool copy(const std::string& src, const std::string& dest, std::string& errorMessage);
bool move(const std::string& src, const std::string& dest, std::string& errorMessage);
bool move_replace(const std::string& src, const std::string& dest, std::string& errorMessage);
void remove(const std::string& path);
} // namespace FileUtil
} // namespace Utils