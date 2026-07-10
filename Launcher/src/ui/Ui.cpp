#include "Ui.hpp"
#include "Config.hpp"
#include "Patcher.hpp"
#include "Utils.hpp"

#include <windows.h>
#include <filesystem>
#include <string>
#include <chrono>
#include <vector>

extern Config g_config;
namespace fs = std::filesystem;

constexpr int GAME_GW1 = 0;
constexpr int GAME_GW2 = 1;
constexpr int GAME_BFN = 2;

static bool DrawGamePathInput(HWND hwnd, const char* label, const char* exeName, std::string& pathRef, int gameType, float dpiScale,
    float centerOffset, float fieldWidth)
{
    bool pathChanged = false;

    ImGui::SetCursorPosX(centerOffset);
    ImGui::TextUnformatted(label);
    ImGui::SetCursorPosX(centerOffset);
    ImGui::SetNextItemWidth(fieldWidth - 180 * dpiScale);

    char buf[512];
    strncpy_s(buf, pathRef.c_str(), sizeof(buf));

    std::string inputId = std::string("##Path_") + label;
    if (ImGui::InputText(inputId.c_str(), buf, sizeof(buf)))
    {
        pathRef = buf;
        pathChanged = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Auto Detect", ImVec2(100 * dpiScale, 0)))
    {
        std::string detected = Utils::Registry::GetGamePathFromRegistry(gameType);
        if (!detected.empty())
        {
            fs::path exePath = fs::path(detected) / exeName;
            pathRef = exePath.string();
            pathChanged = true;
            Utils::UI::Status::Show(std::string("Auto detected path: ") + pathRef);
        }
        else
        {
            Utils::UI::Status::Show("Could not auto detect installation", true);
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Browse", ImVec2(80 * dpiScale, 0)))
    {
        std::string chosen = Utils::Dialog::BrowseForExe(hwnd, gameType);
        if (!chosen.empty())
        {
            pathRef = chosen;
            pathChanged = true;
            Utils::UI::Status::Show(std::string("Path set: ") + chosen);
        }
    }

    return pathChanged;
}

static void setupStyle(float dpiScale)
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
    style.Colors[ImGuiCol_Button] = g_config.baseColor;
    style.Colors[ImGuiCol_ButtonHovered] = g_config.baseHover;
    style.Colors[ImGuiCol_ButtonActive] = g_config.baseActive;
    style.Colors[ImGuiCol_CheckMark] = g_config.baseColor;
    style.Colors[ImGuiCol_FrameBgActive] = g_config.baseActive;
    style.Colors[ImGuiCol_FrameBgHovered] = g_config.baseHover;
    style.Colors[ImGuiCol_Header] = g_config.baseColor;
    style.Colors[ImGuiCol_HeaderHovered] = g_config.baseHover;
    style.Colors[ImGuiCol_HeaderActive] = g_config.baseActive;
    style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

    style.WindowPadding = ImVec2(16 * dpiScale, 10 * dpiScale);
    style.ItemSpacing = ImVec2(10 * dpiScale, 6 * dpiScale);
    style.FramePadding = ImVec2(6 * dpiScale, 4 * dpiScale);
    style.WindowBorderSize = 0.0f;
    style.FrameRounding = 4.0f;
}

static void drawTitleBar(HWND hwnd, float dpiScale)
{
    float h = 40.0f * dpiScale;
    ImGui::BeginChild("TitleBar", ImVec2(0, h), false);

    ImGui::SetCursorPos(ImVec2(15 * dpiScale, (h - ImGui::GetFontSize() * 1.2f) * 0.5f));
    ImGui::TextUnformatted("GARDENGATE LAUNCHER");

    float btnW = 40 * dpiScale, btnH = 26 * dpiScale, spacing = 12 * dpiScale, rightPad = 10 * dpiScale;
    float yOffset = (h - btnH) * 0.5f;
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - (btnW * 2) - spacing - rightPad, yOffset));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.18f, 0.18f, 0.18f, 1));
    if (ImGui::Button("_", ImVec2(btnW, btnH)))
        ::ShowWindow(hwnd, SW_MINIMIZE);
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("X", ImVec2(btnW, btnH)))
        ::PostMessage(hwnd, WM_CLOSE, 0, 0);
    ImGui::PopStyleColor();

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        if (!ImGui::IsAnyItemActive())
        {
            ::ReleaseCapture();
            ::SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        }
    }
    ImGui::EndChild();
}

static void drawTabBar(int& currentTab, float dpiScale)
{
    ImGui::BeginChild("TabBar", ImVec2(0, 45.0f * dpiScale), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::SetCursorPos(ImVec2(15.0f * dpiScale, 6.0f * dpiScale));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20.0f * dpiScale, 8.0f * dpiScale));

    float tabWidth = 130.0f * dpiScale;
    ImVec4 activeColor = g_config.baseColor;
    ImVec4 inactiveColor = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);

    const char* tabs[] = { "Launcher", "Patcher", "Misc" };
    for (int i = 0; i < 3; ++i)
    {
        if (i > 0)
            ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, currentTab == i ? activeColor : inactiveColor);
        if (ImGui::Button(tabs[i], ImVec2(tabWidth, 0)))
            currentTab = i;
        ImGui::PopStyleColor();
    }

    ImGui::PopStyleVar();
    ImGui::EndChild();
}

static void drawStatusMessage(float dpiScale)
{
    if (Utils::UI::Status::GetCurrent().text.empty() || Utils::UI::Status::GetCurrent().isExpired())
        return;

    float safeWidth = ImGui::GetContentRegionAvail().x;
    float centerOffset = (safeWidth - 400 * dpiScale) / 2.0f;
    ImGui::Dummy(ImVec2(0, 5 * dpiScale));

    auto elapsed = std::chrono::steady_clock::now() - Utils::UI::Status::GetCurrent().showTime;
    float elapsedSec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() / 1000.0f;

    float alpha = 1.0f;
    if (elapsedSec > Utils::UI::Status::GetCurrent().duration - 0.5f)
    {
        alpha = (Utils::UI::Status::GetCurrent().duration - elapsedSec) * 2.0f;
    }

    ImVec4 textColor = Utils::UI::Status::GetCurrent().isError ? ImVec4(1.0f, 0.4f, 0.4f, alpha) : ImVec4(0.4f, 1.0f, 0.4f, alpha);

    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    float textWidth = ImGui::CalcTextSize(Utils::UI::Status::GetCurrent().text.c_str()).x;
    float padding = (400 * dpiScale - textWidth) / 2.0f;

    ImGui::SetCursorPosX(centerOffset + padding);
    ImGui::Text("%s", Utils::UI::Status::GetCurrent().text.c_str());
    ImGui::PopStyleColor();
}

static void drawLauncherTab(HWND hwnd, float dpiScale)
{
    float safeWidth = ImGui::GetContentRegionAvail().x;
    float fieldWidth = safeWidth * 0.85f;
    float centerOffset = (safeWidth - fieldWidth) / 2.0f;

    int gameSelectedInt = g_config.get_game_selected_int();

    ImGui::SetCursorPosX(centerOffset);
    if (ImGui::RadioButton("GW1", &gameSelectedInt, GAME_GW1))
        g_config.set_game_selected_from_int(GAME_GW1);
    ImGui::SameLine(0.0f, 40 * dpiScale);
    if (ImGui::RadioButton("GW2", &gameSelectedInt, GAME_GW2))
        g_config.set_game_selected_from_int(GAME_GW2);
    ImGui::SameLine(0.0f, 40 * dpiScale);
    if (ImGui::RadioButton("BFN", &gameSelectedInt, GAME_BFN))
        g_config.set_game_selected_from_int(GAME_BFN);

    ImGui::Separator();
    Utils::UI::CenteredInput("Username:", g_config.username, centerOffset, fieldWidth);
    Utils::UI::CenteredInput("Server IP:", g_config.server_ip, centerOffset, fieldWidth);
    Utils::UI::CenteredInput("Password:", g_config.password, centerOffset, fieldWidth, ImGuiInputTextFlags_Password);

    auto& currentGame = g_config.get_current_game();

    const char* exeNames[] = { "PVZ.Main_Win64_Retail.exe", "GW2.Main_Win64_Retail.exe", "PVZBattleforNeighborville.exe" };
    if (DrawGamePathInput(
            hwnd, "Game Path:", exeNames[gameSelectedInt], currentGame.game_path, gameSelectedInt, dpiScale, centerOffset, fieldWidth))
    {
        save_config(g_config, "config.json");
    }

    ImGui::Dummy(ImVec2(0, 10 * dpiScale));
    ImGui::SetCursorPosX(centerOffset);

    if (ImGui::Checkbox("Use ModData", &currentGame.moddata_enabled))
    {
        save_config(g_config, "config.json");
    }

    ImGui::SameLine(0.0f, 40 * dpiScale);

    if (ImGui::Checkbox("Launch directly from EXE", &g_config.launch_directly))
    {
        save_config(g_config, "config.json");
    }

    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Use direct EXE launch instead of Steam/EA Desktop");
    }

    if (currentGame.moddata_enabled)
    {
        std::vector<std::string> modFolders = { "Default" };
        if (!currentGame.game_path.empty())
        {
            fs::path modDataDir = fs::path(currentGame.game_path).parent_path() / "ModData";
            if (fs::exists(modDataDir) && fs::is_directory(modDataDir))
            {
                for (const auto& entry : fs::directory_iterator(modDataDir))
                {
                    if (entry.is_directory() && entry.path().filename() != "Default")
                    {
                        modFolders.push_back(entry.path().filename().string());
                    }
                }
            }
            else
            {
                Utils::UI::Status::Show("ModData folder not found at: " + modDataDir.string(), true);
            }
        }

        ImGui::SetCursorPosX(centerOffset);
        ImGui::TextUnformatted("ModData Preset:");
        ImGui::SetCursorPosX(centerOffset);
        ImGui::SetNextItemWidth(fieldWidth);

        std::vector<const char*> modCStrings;
        int currentSelection = 0;
        for (size_t i = 0; i < modFolders.size(); ++i)
        {
            modCStrings.push_back(modFolders[i].c_str());
            if (modFolders[i] == currentGame.moddata_selected)
                currentSelection = (int)i;
        }

        if (ImGui::Combo("##ModDataPreset", &currentSelection, modCStrings.data(), (int)modCStrings.size()))
        {
            currentGame.moddata_selected = modFolders[currentSelection];
            save_config(g_config, "config.json");
            Utils::UI::Status::Show("ModData preset set to: " + currentGame.moddata_selected);
        }
    }

    Utils::UI::CenteredInput("Custom Arguments:", currentGame.custom_args, centerOffset, fieldWidth);

    std::string args = Utils::Process::BuildArgs(g_config);

    ImGui::Dummy(ImVec2(0, 8 * dpiScale));
    ImGui::SetCursorPosX(centerOffset);
    ImGui::TextWrapped(("Args preview: " + args).c_str());

    ImGui::Dummy(ImVec2(0, 12 * dpiScale));
    if (Utils::UI::CenteredButton("LAUNCH GAME", 180 * dpiScale, dpiScale))
    {
        save_config(g_config, "config.json");

        if (currentGame.game_path.empty())
        {
            Utils::UI::Status::Show("Please set a game path first!", true);
        }
        else
        {
            std::string modDataPath =
                (currentGame.moddata_enabled && !currentGame.moddata_selected.empty()) ? "ModData/" + currentGame.moddata_selected : "";

            Utils::Process::PatchEAArgs(args, gameSelectedInt);
            auto lr =
                Utils::Process::LaunchAndInject(currentGame.game_path, args, "level_loader.dll", modDataPath, gameSelectedInt, g_config);

            if (!lr.ok)
            {
                Utils::UI::Status::Show("Failed to launch: " + lr.error, true);
            }
            else
            {
                Utils::UI::Status::Show("Launching game...\nArgs: " + args);
                CloseHandle(lr.pi.hProcess);
                CloseHandle(lr.pi.hThread);
            }
        }
    }

    drawStatusMessage(dpiScale);
}

static void drawPatcherTab(HWND hwnd, float dpiScale)
{
    float safeWidth = ImGui::GetContentRegionAvail().x;
    float fieldWidth = safeWidth * 0.85f;
    float centerOffset = (safeWidth - fieldWidth) / 2.0f;

    int gameSelectedInt = g_config.get_game_selected_int();

    fs::path launcherDir = fs::path([] {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        return std::string(path);
    }())
                               .parent_path();

    std::string dllFile = (launcherDir / "dinput8.dll").string();

    if (gameSelectedInt == GAME_GW1)
    {
        auto& gw1Config = g_config.gw1;

        if (DrawGamePathInput(
                hwnd, "GW1 Executable:", "PVZ.Main_Win64_Retail.exe", gw1Config.game_path, GAME_GW1, dpiScale, centerOffset, fieldWidth))
        {
            save_config(g_config, "config.json");
        }

        ImGui::Dummy(ImVec2(0, 20 * dpiScale));

        if (!gw1Config.game_path.empty() && fs::exists(gw1Config.game_path))
        {
            bool isPatched = Patcher::IsGW1Patched(gw1Config.game_path);
            fs::path gameDir = fs::path(gw1Config.game_path).parent_path();
            fs::path dllPath = gameDir / "dinput8.dll";
            fs::path dllDisabled = gameDir / "dinput8.dll.disabled";

            if (isPatched)
            {
                ImGui::SetCursorPosX(centerOffset);
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "Garden Gate is installed");

                if (Utils::UI::CenteredButton("DISABLE GARDEN GATE", 200 * dpiScale, dpiScale))
                {
                    std::string err;
                    if (Patcher::RestoreGW1(gw1Config.game_path, err))
                    {
                        Utils::UI::Status::Show("Garden Gate disabled for GW1");
                    }
                    else
                    {
                        Utils::UI::Status::Show("Error: " + err, true);
                    }
                }
            }
            else if (fs::exists(dllDisabled))
            {
                ImGui::SetCursorPosX(centerOffset);
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Garden Gate is disabled");

                if (Utils::UI::CenteredButton("ENABLE GARDEN GATE", 200 * dpiScale, dpiScale))
                {
                    std::string err;
                    if (Patcher::AutoPatchGW1(gw1Config.game_path, dllFile, err))
                    {
                        Utils::UI::Status::Show("Garden Gate enabled for GW1");
                    }
                    else
                    {
                        Utils::UI::Status::Show("Error: " + err, true);
                    }
                }
            }
            else
            {
                ImGui::SetCursorPosX(centerOffset);
                ImGui::Text("Garden Gate is not installed");

                if (Utils::UI::CenteredButton("INSTALL GARDEN GATE", 200 * dpiScale, dpiScale))
                {
                    std::string err;
                    if (Patcher::AutoPatchGW1(gw1Config.game_path, dllFile, err))
                    {
                        Utils::UI::Status::Show("Garden Gate installed for gw1");
                    }
                    else
                    {
                        Utils::UI::Status::Show("Error: " + err, true);
                    }
                }
            }
        }
        else
        {
            ImGui::SetCursorPosX(centerOffset);
            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Set GW1 path first!");
        }
    }
    else
    {
        bool isGW2 = (gameSelectedInt == GAME_GW2);
        auto& gameConfig = isGW2 ? g_config.gw2 : g_config.bfn;
        const char* label = isGW2 ? "GW2 Executable:" : "BFN Executable:";
        const char* exeName = isGW2 ? "GW2.Main_Win64_Retail.exe" : "PVZBattleforNeighborville.exe";
        const char* patchSubDir = isGW2 ? "gw2" : "bfn";
        const char* patchFileName = isGW2 ? "GW2.Main_Win64_Retail.exe.xdelta" : "PVZBattleforNeighborville.exe.xdelta";

        bool isPatched = !gameConfig.game_path.empty() &&
                         (isGW2 ? Patcher::IsGW2Patched(gameConfig.game_path) : Patcher::IsBFNPatched(gameConfig.game_path));

        if (DrawGamePathInput(hwnd, label, exeName, gameConfig.game_path, gameSelectedInt, dpiScale, centerOffset, fieldWidth))
        {
            save_config(g_config, "config.json");
            isPatched = isGW2 ? Patcher::IsGW2Patched(gameConfig.game_path) : Patcher::IsBFNPatched(gameConfig.game_path);
        }

        ImGui::Dummy(ImVec2(0, 20 * dpiScale));

        std::string patchFile = (launcherDir / "patches" / patchSubDir / patchFileName).string();
        bool hasPatchFiles = fs::exists(patchFile);
        bool hasDLL = fs::exists(dllFile);

        if (!hasPatchFiles || !hasDLL)
        {
            ImGui::SetCursorPosX(centerOffset);
            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Missing required files:");
            if (!hasPatchFiles)
            {
                ImGui::SetCursorPosX(centerOffset);
                ImGui::Text("- Patch files in patches/%s/", patchSubDir);
            }
            if (!hasDLL)
            {
                ImGui::SetCursorPosX(centerOffset);
                ImGui::Text("- dinput8.dll");
            }
        }

        if (isPatched)
        {
            ImGui::SetCursorPosX(centerOffset);
            ImGui::TextColored(ImVec4(0, 1, 0, 1), isGW2 ? "GW2 is already patched" : "BFN is already patched");

            if (Utils::UI::CenteredButton(isGW2 ? "RESTORE ORIGINAL GW2" : "RESTORE ORIGINAL BFN", 200 * dpiScale, dpiScale))
            {
                std::string err;
                bool restored = isGW2 ? Patcher::RestoreGW2(gameConfig.game_path, err) : Patcher::RestoreBFN(gameConfig.game_path, err);
                if (restored)
                {
                    isPatched = false;
                    Utils::UI::Status::Show(isGW2 ? "GW2 restored to original" : "BFN restored to original");
                }
                else
                {
                    Utils::UI::Status::Show("Error: " + err, true);
                }
            }
        }
        else
        {
            ImGui::SetCursorPosX(centerOffset);
            ImGui::Text("One click patch for %s", isGW2 ? "Garden Warfare 2" : "Battle for Neighborville");
            ImGui::BeginDisabled(!hasPatchFiles || !hasDLL || gameConfig.game_path.empty());

            if (Utils::UI::CenteredButton(isGW2 ? "AUTO PATCH GW2" : "AUTO PATCH BFN", 200 * dpiScale, dpiScale))
            {
                std::string err;
                bool patched = isGW2
                                   ? Patcher::AutoPatchGW2(gameConfig.game_path, patchFile, dllFile, err)
                                   : Patcher::AutoPatchBFN(gameConfig.game_path, (launcherDir / "patches" / "bfn").string(), dllFile, err);

                if (patched)
                {
                    isPatched = true;
                    Utils::UI::Status::Show(isGW2 ? "GW2 patched successfully!" : "BFN patched successfully!");
                }
                else
                {
                    Utils::UI::Status::Show("Error: " + err, true);
                }
            }
            ImGui::EndDisabled();
        }
    }

    drawStatusMessage(dpiScale);
}

static void drawMiscTab(HWND hwnd, float dpiScale)
{
    float safeWidth = ImGui::GetContentRegionAvail().x;

    ImGui::Dummy(ImVec2(0, 30 * dpiScale));
    ImGui::SetWindowFontScale(1.3f);
    ImGui::PushStyleColor(ImGuiCol_Text, g_config.baseColor);
    Utils::UI::CenteredText("CREDITS", safeWidth);
    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(0, 15 * dpiScale));

    const std::vector<std::string> credits = { "RaT", "nocss", "twig", "blueballoon", "eshaydev", "gargos69junior", "werzdragon",
        "megadwarf47" };
    float nameSpacing = 25.0f * dpiScale;
    float totalCreditsWidth = 0.0f;

    for (const auto& name : credits)
        totalCreditsWidth += ImGui::CalcTextSize(name.c_str()).x;
    if (credits.size() > 1)
        totalCreditsWidth += (nameSpacing * (credits.size() - 1));

    ImGui::SetCursorPosX((safeWidth - totalCreditsWidth) * 0.5f);
    for (size_t i = 0; i < credits.size(); i++)
    {
        if (i > 0)
            ImGui::SameLine(0, nameSpacing);
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), credits[i].c_str());
    }

    ImGui::SetWindowFontScale(1.0f);
    ImGui::Dummy(ImVec2(0, 40 * dpiScale));

    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
    ImGui::Separator();
    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(0, 20 * dpiScale));

    ImGui::SetWindowFontScale(1.3f);
    ImGui::PushStyleColor(ImGuiCol_Text, g_config.baseColor);
    Utils::UI::CenteredText("UI THEME", safeWidth);
    ImGui::PopStyleColor();
    ImGui::SetWindowFontScale(1.0f);
    ImGui::Dummy(ImVec2(0, 15 * dpiScale));

    static const char* presetNames[] = { "Default", "Purple", "Blue", "Red" };
    static const char* displayNames[] = { "Default", "Purple", "Blue", "Red", "Custom" };
    static int currentPreset = 0;

    float comboWidth = 200 * dpiScale;
    ImGui::SetCursorPosX((safeWidth - comboWidth) * 0.5f);
    ImGui::SetNextItemWidth(comboWidth);

    const char* previewValue = (currentPreset >= 0 && currentPreset < 4) ? displayNames[currentPreset] : displayNames[4];

    if (ImGui::BeginCombo("##Presets", previewValue))
    {
        for (int i = 0; i < 4; i++)
        {
            bool isSelected = (currentPreset == i);
            if (ImGui::Selectable(presetNames[i], isSelected))
            {
                currentPreset = i;
                switch (currentPreset)
                {
                case 0:
                    g_config.baseColor = ImVec4(0.25f, 0.47f, 0.32f, 1.0f);
                    g_config.baseHover = ImVec4(0.30f, 0.55f, 0.38f, 1.0f);
                    g_config.baseActive = ImVec4(0.20f, 0.40f, 0.28f, 1.0f);
                    break;
                case 1:
                    g_config.baseColor = ImVec4(0.6f, 0.0f, 0.8f, 1.0f);
                    g_config.baseHover = ImVec4(0.5f, 0.0f, 0.7f, 1.0f);
                    g_config.baseActive = ImVec4(0.4f, 0.0f, 0.6f, 1.0f);
                    break;
                case 2:
                    g_config.baseColor = ImVec4(0.0f, 0.5f, 0.9f, 1.0f);
                    g_config.baseHover = ImVec4(0.0f, 0.4f, 0.8f, 1.0f);
                    g_config.baseActive = ImVec4(0.0f, 0.3f, 0.7f, 1.0f);
                    break;
                case 3:
                    g_config.baseColor = ImVec4(0.8f, 0.1f, 0.1f, 1.0f);
                    g_config.baseHover = ImVec4(0.7f, 0.1f, 0.1f, 1.0f);
                    g_config.baseActive = ImVec4(0.6f, 0.1f, 0.1f, 1.0f);
                    break;
                }
                setupStyle(dpiScale);
                save_config(g_config, "config.json");
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Dummy(ImVec2(0, 25 * dpiScale));

    float pickerSize = 50.0f * dpiScale, spacing = 40.0f * dpiScale, textGap = 4.0f * dpiScale;
    float totalBlockWidth = (pickerSize * 3) + (spacing * 2);
    float startX = (safeWidth - totalBlockWidth) * 0.5f;
    float startY = ImGui::GetCursorPosY();

    auto DrawCenteredItem = [&](int index, const char* label, ImVec4& color, const char* id) {
        ImGui::PushID(id);
        ImGui::SetCursorPos(ImVec2(startX + (index * (pickerSize + spacing)), startY));
        ImGui::SetNextItemWidth(pickerSize);
        if (ImGui::ColorEdit3("", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
        {
            currentPreset = -1;
            setupStyle(dpiScale);
            save_config(g_config, "config.json");
        }

        ImVec2 boxPos = ImGui::GetItemRectMin();
        ImVec2 boxSize = ImGui::GetItemRectSize();
        ImVec2 textSize = ImGui::CalcTextSize(label);
        ImVec2 textPos(boxPos.x + (boxSize.x * 0.5f) - (textSize.x * 0.5f), boxPos.y - textSize.y - 6.0f * dpiScale);

        ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(180, 180, 180, 255), label);
        ImGui::PopID();
    };

    DrawCenteredItem(0, "Base", g_config.baseColor, "BaseColor");
    DrawCenteredItem(1, "Hover", g_config.baseHover, "HoverColor");
    DrawCenteredItem(2, "Active", g_config.baseActive, "ActiveColor");

    ImGui::SetCursorPosY(startY + ImGui::GetTextLineHeight() + textGap + ImGui::GetFrameHeight());
    ImGui::Dummy(ImVec2(0, 50 * dpiScale));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

#ifdef GIT_HASH
    #ifdef GIT_DATE
    Utils::UI::CenteredText("Launcher Version: " + std::string(GIT_HASH) + " (" + GIT_DATE + ")", safeWidth);
    #else
    Utils::UI::CenteredText(GIT_HASH, safeWidth);
    #endif
#else
    Utils::UI::CenteredText("unknown", safeWidth);
#endif

    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(0, 10 * dpiScale));

    if (Utils::UI::CenteredButton("GITHUB REPOSITORY", 220 * dpiScale, dpiScale))
    {
        ShellExecuteA(hwnd, "open", "https://github.com/nocss42/GardenGate", NULL, NULL, SW_SHOWNORMAL);
    }

    drawStatusMessage(dpiScale);
}

void UI::DrawUI(HWND hwnd, float dpiScale)
{
    static int currentTab = 0;
    setupStyle(dpiScale);

    RECT rc;
    GetClientRect(hwnd, &rc);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)(rc.right - rc.left), (float)(rc.bottom - rc.top)));

    ImGui::Begin("Garden Gate", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    drawTitleBar(hwnd, dpiScale);
    drawTabBar(currentTab, dpiScale);

    ImGui::BeginChild("ContentArea", ImVec2(0, ImGui::GetContentRegionAvail().y), false);

    switch (currentTab)
    {
    case 0:
        drawLauncherTab(hwnd, dpiScale);
        break;
    case 1:
        drawPatcherTab(hwnd, dpiScale);
        break;
    case 2:
        drawMiscTab(hwnd, dpiScale);
        break;
    }

    ImGui::EndChild();
    ImGui::End();
}