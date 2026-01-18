#include "Ui.hpp"
#include "Config.hpp"
#include "Patcher.hpp"
#include "Utils.hpp"

#include <windows.h>
#include <filesystem>
#include <string>
#include <chrono>

extern Config g_config;
namespace fs = std::filesystem;

struct StatusMessage {
	std::string text;
	bool isError;
	std::chrono::steady_clock::time_point showTime;
	float duration;

	StatusMessage(const std::string& t = "", bool error = false, float dur = 3.0f)
		: text(t), isError(error), showTime(std::chrono::steady_clock::now()), duration(dur) {
	}

	bool isExpired() const {
		auto elapsed = std::chrono::steady_clock::now() - showTime;
		return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() > duration * 1000;
	}
};

static StatusMessage currentStatus;

static void showStatus(const std::string& text, bool isError = false, float duration = 3.0f) {
	currentStatus = StatusMessage(text, isError, duration);
}

static void setupStyle(float dpiScale) {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 baseGreen = ImVec4(0.25f, 0.47f, 0.32f, 1.0f);
	ImVec4 hoverGreen = ImVec4(0.30f, 0.55f, 0.38f, 1.0f);
	ImVec4 activeGreen = ImVec4(0.20f, 0.40f, 0.28f, 1.0f);

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
	style.Colors[ImGuiCol_Button] = baseGreen;
	style.Colors[ImGuiCol_ButtonHovered] = hoverGreen;
	style.Colors[ImGuiCol_ButtonActive] = activeGreen;
	style.Colors[ImGuiCol_CheckMark] = baseGreen;
	style.Colors[ImGuiCol_FrameBgActive] = activeGreen;
	style.Colors[ImGuiCol_FrameBgHovered] = hoverGreen;
	style.Colors[ImGuiCol_Header] = baseGreen;
	style.Colors[ImGuiCol_HeaderHovered] = hoverGreen;
	style.Colors[ImGuiCol_HeaderActive] = activeGreen;
	style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

	style.WindowPadding = ImVec2(16 * dpiScale, 10 * dpiScale);
	style.ItemSpacing = ImVec2(10 * dpiScale, 6 * dpiScale);
	style.FramePadding = ImVec2(6 * dpiScale, 4 * dpiScale);
	style.WindowBorderSize = 0.0f;
	style.FrameRounding = 4.0f;
}

static void drawTitleBar(HWND hwnd, float dpiScale) {
	float h = 40.0f * dpiScale;
	ImGui::BeginChild("TitleBar", ImVec2(0, h), false);

	ImGui::SetCursorPos(ImVec2(15 * dpiScale, (h - ImGui::GetFontSize() * 1.2f) * 0.5f));
	ImGui::TextUnformatted("GARDENGATE LAUNCHER");

	float btnW = 40 * dpiScale, btnH = 26 * dpiScale, spacing = 12 * dpiScale, rightPad = 10 * dpiScale;
	float yOffset = (h - btnH) * 0.5f;
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - (btnW * 2) - spacing - rightPad, yOffset));

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.18f, 0.18f, 0.18f, 1));
	if (ImGui::Button("_", ImVec2(btnW, btnH))) ::ShowWindow(hwnd, SW_MINIMIZE);
	ImGui::SameLine(0, spacing);
	if (ImGui::Button("X", ImVec2(btnW, btnH))) ::PostMessage(hwnd, WM_CLOSE, 0, 0);
	ImGui::PopStyleColor();

	if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		if (!ImGui::IsAnyItemActive()) {
			::ReleaseCapture();
			::SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}
	}
	ImGui::EndChild();
}

static void drawTabBar(int& currentTab, float dpiScale) {
	ImVec4 colorBaseGreen = ImVec4(0.25f, 0.47f, 0.32f, 1.0f);

	ImGui::BeginChild("TabBar", ImVec2(0, 45.0f * dpiScale), false,
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::SetCursorPos(ImVec2(15.0f * dpiScale, 6.0f * dpiScale));

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20.0f * dpiScale, 8.0f * dpiScale));

	float tabWidth = 130.0f * dpiScale;

	ImGui::PushStyleColor(ImGuiCol_Button,
		currentTab == 0 ? colorBaseGreen : ImVec4(0.18f, 0.18f, 0.18f, 1.0f));
	if (ImGui::Button("Launcher", ImVec2(tabWidth, 0))) currentTab = 0;
	ImGui::PopStyleColor();

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button,
		currentTab == 1 ? colorBaseGreen : ImVec4(0.18f, 0.18f, 0.18f, 1.0f));
	if (ImGui::Button("Patcher", ImVec2(tabWidth, 0))) currentTab = 1;
	ImGui::PopStyleColor();

	ImGui::PopStyleVar();
	ImGui::EndChild();
}

static void drawStatusMessage(float dpiScale) {
	if (currentStatus.text.empty() || currentStatus.isExpired()) {
		return;
	}

	float safeWidth = ImGui::GetContentRegionAvail().x;
	float centerOffset = (safeWidth - 400 * dpiScale) / 2.0f;

	ImGui::Dummy(ImVec2(0, 5 * dpiScale));

	auto elapsed = std::chrono::steady_clock::now() - currentStatus.showTime;
	float elapsedSec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() / 1000.0f;
	float alpha = 1.0f;

	if (elapsedSec > currentStatus.duration - 0.5f) {
		alpha = (currentStatus.duration - elapsedSec) * 2.0f;
	}

	ImVec4 textColor = currentStatus.isError ?
		ImVec4(1.0f, 0.4f, 0.4f, alpha) :
		ImVec4(0.4f, 1.0f, 0.4f, alpha);

	ImGui::SetCursorPosX(centerOffset);
	ImGui::PushStyleColor(ImGuiCol_Text, textColor);

	float textWidth = ImGui::CalcTextSize(currentStatus.text.c_str()).x;
	float padding = (400 * dpiScale - textWidth) / 2.0f;

	ImGui::SetCursorPosX(centerOffset + padding);
	ImGui::Text("%s", currentStatus.text.c_str());
	ImGui::PopStyleColor();
}

static void drawLauncherTab(HWND hwnd, float dpiScale) {
	float safeWidth = ImGui::GetContentRegionAvail().x;
	float fieldWidth = safeWidth * 0.85f;
	float centerOffset = (safeWidth - fieldWidth) / 2.0f;

	int  gameSelectedInt = g_config.get_game_selected_int();
	bool isGW1 = (gameSelectedInt == 0);

	ImGui::SetCursorPosX(centerOffset);
	if (ImGui::RadioButton("GW1", &gameSelectedInt, 0)) g_config.set_game_selected_from_int(0);
	ImGui::SameLine(0.0f, 40 * dpiScale);
	if (ImGui::RadioButton("GW2", &gameSelectedInt, 1)) g_config.set_game_selected_from_int(1);
	ImGui::SameLine(0.0f, 40 * dpiScale);
	if (ImGui::RadioButton("BFN", &gameSelectedInt, 2)) g_config.set_game_selected_from_int(2);

	ImGui::Separator();
	Utils::UI::CenteredInput("Username:", g_config.username, centerOffset, fieldWidth);
	Utils::UI::CenteredInput("Server IP:", g_config.server_ip, centerOffset, fieldWidth);
	Utils::UI::CenteredInput("Password:", g_config.password, centerOffset, fieldWidth, ImGuiInputTextFlags_Password);

	auto& currentGame = g_config.get_current_game();
	ImGui::SetCursorPosX(centerOffset);
	ImGui::TextUnformatted("Game Path:");
	ImGui::SetCursorPosX(centerOffset);
	ImGui::SetNextItemWidth(fieldWidth - 180 * dpiScale);

	char buf[512];
	strncpy_s(buf, currentGame.game_path.c_str(), sizeof(buf));
	if (ImGui::InputText("##GamePath", buf, sizeof(buf))) currentGame.game_path = buf;

	ImGui::SameLine();
	if (ImGui::Button("AutoDetect", ImVec2(100 * dpiScale, 0))) {
		std::string detected = Utils::Registry::GetGamePathFromRegistry(gameSelectedInt);
		if (!detected.empty()) {
			fs::path exePath = fs::path(detected);
			if (gameSelectedInt == 0) {
				exePath /= "PVZ.Main_Win64_Retail.exe";
			}
			else if (gameSelectedInt == 1) {
				exePath /= "GW2.Main_Win64_Retail.exe";
			}
			else {
				exePath /= "PVZBattleforNeighborville.exe";
			}
			currentGame.game_path = exePath.string();
			showStatus("Auto detected game path: " + currentGame.game_path);
		}
		else {
			showStatus("Could not auto detect game installation", true);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Browse", ImVec2(80 * dpiScale, 0))) {
		std::string chosen = Utils::Dialog::BrowseForExe(hwnd, gameSelectedInt);
		if (!chosen.empty()) {
			currentGame.game_path = chosen;
			showStatus("Game path set: " + chosen);
		}
	}

	ImGui::Dummy(ImVec2(0, 10 * dpiScale));
	ImGui::SetCursorPosX(centerOffset);
	if (ImGui::Checkbox("Use ModData", &currentGame.moddata_enabled)) {
		save_config(g_config, "config.json");
	}

	if (currentGame.moddata_enabled) {
		std::vector<std::string> modFolders = { "Default" };
		if (!currentGame.game_path.empty()) {
			fs::path gamePath = fs::path(currentGame.game_path);
			if (fs::exists(gamePath)) {
				fs::path modDataDir = gamePath.parent_path() / "ModData";
				if (fs::exists(modDataDir) && fs::is_directory(modDataDir)) {
					for (const auto& entry : fs::directory_iterator(modDataDir)) {
						if (entry.is_directory()) {
							std::string folderName = entry.path().filename().string();
							if (folderName != "Default") modFolders.push_back(folderName);
						}
					}
				}
				else {
					showStatus("ModData folder not found at: " + modDataDir.string(), true);
				}
			}
		}

		ImGui::SetCursorPosX(centerOffset);
		ImGui::TextUnformatted("ModData Preset:");
		ImGui::SetCursorPosX(centerOffset);
		ImGui::SetNextItemWidth(fieldWidth);

		std::vector<const char*> modCStrings;
		modCStrings.reserve(modFolders.size());
		for (const auto& mod : modFolders) modCStrings.push_back(mod.c_str());

		int currentSelection = 0;
		for (size_t i = 0; i < modFolders.size(); ++i) {
			if (modFolders[i] == currentGame.moddata_selected) { currentSelection = (int)i; break; }
		}

		if (ImGui::Combo("##ModDataPreset", &currentSelection, modCStrings.data(), (int)modCStrings.size())) {
			currentGame.moddata_selected = modFolders[currentSelection];
			save_config(g_config, "config.json");
			showStatus("ModData preset set to: " + currentGame.moddata_selected);
		}
	}

	Utils::UI::CenteredInput("Custom Arguments:", currentGame.custom_args, centerOffset, fieldWidth);

	std::string args = Utils::Process::BuildArgs(g_config);

	ImGui::Dummy(ImVec2(0, 8 * dpiScale));
	ImGui::SetCursorPosX(centerOffset);
	ImGui::TextWrapped(("Args preview: " + args).c_str());

	ImGui::Dummy(ImVec2(0, 12 * dpiScale));
	if (Utils::UI::CenteredButton("LAUNCH GAME", 180 * dpiScale, dpiScale)) {
		save_config(g_config, "config.json");

		if (currentGame.game_path.empty()) {
			showStatus("Please set a game path first!", true);
		}
		else {
			std::string modDataPath;
			if (currentGame.moddata_enabled && !currentGame.moddata_selected.empty()) {
				modDataPath = "ModData/" + currentGame.moddata_selected;
			}

			int gameType = g_config.get_game_selected_int();

			Utils::Process::PatchEAArgs(args, gameType);
			auto lr = Utils::Process::LaunchAndInject(currentGame.game_path, args, "level_loader.dll", modDataPath, gameType);

			if (!lr.ok) {
				showStatus("Failed to launch: " + lr.error, true);
			}
			else {
				showStatus("Launching game...\nArgs: " + args);
				CloseHandle(lr.pi.hProcess);
				CloseHandle(lr.pi.hThread);
			}
		}
	}

	drawStatusMessage(dpiScale);
}

static void drawPatcherTab(HWND hwnd, float dpiScale) {
	float safeWidth = ImGui::GetContentRegionAvail().x;
	float fieldWidth = safeWidth * 0.85f;
	float centerOffset = (safeWidth - fieldWidth) / 2.0f;

	int gameSelectedInt = g_config.get_game_selected_int();

	if (gameSelectedInt == 0) {
		auto& gw1Config = g_config.gw1;

		ImGui::SetCursorPosX(centerOffset);
		ImGui::TextUnformatted("GW1 Executable:");
		ImGui::SetCursorPosX(centerOffset);
		ImGui::SetNextItemWidth(fieldWidth - 180 * dpiScale);

		char buf[512];
		strncpy_s(buf, gw1Config.game_path.c_str(), sizeof(buf));
		if (ImGui::InputText("##GW1ExePath", buf, sizeof(buf))) {
			gw1Config.game_path = buf;
			save_config(g_config, "config.json");
		}

		ImGui::SameLine();
		if (ImGui::Button("Auto Detect", ImVec2(100 * dpiScale, 0))) {
			std::string detected = Utils::Registry::GetGamePathFromRegistry(0);
			if (!detected.empty()) {
				fs::path exePath = fs::path(detected);
				exePath /= "PVZ.Main_Win64_Retail.exe";
				gw1Config.game_path = exePath.string();
				save_config(g_config, "config.json");
				showStatus("Auto detected GW1 path: " + exePath.string());
			}
			else {
				showStatus("Could not auto detect GW1 installation", true);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Browse", ImVec2(80 * dpiScale, 0))) {
			std::string chosen = Utils::Dialog::BrowseForExe(hwnd, 0);
			if (!chosen.empty()) {
				gw1Config.game_path = chosen;
				save_config(g_config, "config.json");
				showStatus("GW1 path set: " + chosen);
			}
		}

		ImGui::Dummy(ImVec2(0, 20 * dpiScale));

		if (!gw1Config.game_path.empty() && fs::exists(gw1Config.game_path)) {
			fs::path gameDir = fs::path(gw1Config.game_path).parent_path();
			fs::path dllPath = gameDir / "dinput8.dll";
			fs::path dllDisabled = gameDir / "dinput8.dll.disabled";

			if (fs::exists(dllPath)) {
				if (Utils::UI::CenteredButton("DISABLE GARDEN GATE", 200 * dpiScale, dpiScale)) {
					std::string err;
					if (Utils::FileUtil::move_replace(dllPath.string(), dllDisabled.string(), err)) {
						showStatus("Garden Gate disabled for GW1");
					}
					else {
						showStatus("Error: " + err, true);
					}
				}
			}
			else if (fs::exists(dllDisabled)) {
				if (Utils::UI::CenteredButton("ENABLE GARDEN GATE", 200 * dpiScale, dpiScale)) {
					std::string err;
					if (Utils::FileUtil::move_replace(dllDisabled.string(), dllPath.string(), err)) {
						showStatus("Garden Gate enabled for GW1");
					}
					else {
						showStatus("Error: " + err, true);
					}
				}
			}
			else {
				if (Utils::UI::CenteredButton("INSTALL GARDEN GATE", 200 * dpiScale, dpiScale)) {
					fs::path launcherDir = fs::path([] {
						char path[MAX_PATH];
						GetModuleFileNameA(NULL, path, MAX_PATH);
						return std::string(path);
						}()).parent_path();
					fs::path dllSource = launcherDir / "dinput8.dll";
					std::string err;
					if (Utils::FileUtil::copy(dllSource.string(), dllPath.string(), err)) {
						showStatus("Garden Gate installed for GW1");
					}
					else {
						showStatus("Error: " + err, true);
					}
				}
			}
		}
		else {
			ImGui::SetCursorPosX(centerOffset);
			ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Set GW1 path first!");
		}
	}
	else if (gameSelectedInt == 1) {
		auto& gw2Config = g_config.gw2;
		bool isPatched = !gw2Config.game_path.empty() && Patcher::IsGW2Patched(gw2Config.game_path);

		ImGui::SetCursorPosX(centerOffset);
		ImGui::TextUnformatted("GW2 Executable:");
		ImGui::SetCursorPosX(centerOffset);
		ImGui::SetNextItemWidth(fieldWidth - 180 * dpiScale);

		char buf[512];
		strncpy_s(buf, gw2Config.game_path.c_str(), sizeof(buf));
		if (ImGui::InputText("##GW2ExePath", buf, sizeof(buf))) {
			gw2Config.game_path = buf;
			save_config(g_config, "config.json");
			if (!gw2Config.game_path.empty()) isPatched = Patcher::IsGW2Patched(gw2Config.game_path);
		}

		ImGui::SameLine();
		if (ImGui::Button("Auto Detect", ImVec2(100 * dpiScale, 0))) {
			std::string detected = Utils::Registry::GetGamePathFromRegistry(1);
			if (!detected.empty()) {
				fs::path exePath = fs::path(detected);
				exePath /= "GW2.Main_Win64_Retail.exe";
				gw2Config.game_path = exePath.string();
				save_config(g_config, "config.json");
				showStatus("Auto detected GW2 path: " + exePath.string());
				isPatched = Patcher::IsGW2Patched(gw2Config.game_path);
			}
			else {
				showStatus("Could not auto detect GW2 installation", true);
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Browse", ImVec2(80 * dpiScale, 0))) {
			std::string chosen = Utils::Dialog::BrowseForExe(hwnd, 1);
			if (!chosen.empty()) {
				gw2Config.game_path = chosen;
				save_config(g_config, "config.json");
				showStatus("GW2 path set: " + chosen);
				isPatched = Patcher::IsGW2Patched(gw2Config.game_path);
			}
		}

		ImGui::Dummy(ImVec2(0, 20 * dpiScale));
		fs::path launcherDir = fs::path([] {
			char path[MAX_PATH];
			GetModuleFileNameA(NULL, path, MAX_PATH);
			return std::string(path);
			}()).parent_path();

		std::string patchFile = (launcherDir / "patches" / "gw2" / "GW2.Main_Win64_Retail.exe.xdelta").string();
		std::string dllFile = (launcherDir / "dinput8.dll").string();
		bool hasPatch = fs::exists(patchFile), hasDLL = fs::exists(dllFile);

		if (!hasPatch || !hasDLL) {
			ImGui::SetCursorPosX(centerOffset);
			ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Missing required files:");
			if (!hasPatch) {
				ImGui::SetCursorPosX(centerOffset);
				ImGui::Text("- GW2 patch files in patches/gw2/");
			}
			if (!hasDLL) {
				ImGui::SetCursorPosX(centerOffset);
				ImGui::Text("- dinput8.dll");
			}
		}

		if (isPatched) {
			ImGui::SetCursorPosX(centerOffset);
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "GW2 is already patched");
			if (Utils::UI::CenteredButton("RESTORE ORIGINAL GW2", 200 * dpiScale, dpiScale)) {
				std::string err;
				if (Patcher::RestoreGW2(gw2Config.game_path, err)) {
					isPatched = false;
					showStatus("GW2 restored to original");
				}
				else {
					showStatus("Error: " + err, true);
				}
			}
		}
		else {
			ImGui::SetCursorPosX(centerOffset);
			ImGui::Text("One click patch for Garden Warfare 2");
			ImGui::BeginDisabled(!hasPatch || !hasDLL || gw2Config.game_path.empty());
			if (Utils::UI::CenteredButton("AUTO PATCH GW2", 200 * dpiScale, dpiScale)) {
				std::string err;
				if (Patcher::AutoPatchGW2(gw2Config.game_path, patchFile, dllFile, err)) {
					isPatched = true;
					showStatus("GW2 patched successfully!");
				}
				else {
					showStatus("Error: " + err, true);
				}
			}
			ImGui::EndDisabled();
		}
	}
	else if (gameSelectedInt == 2) {
		auto& bfnConfig = g_config.bfn;
		bool isPatched = !bfnConfig.game_path.empty() && Patcher::IsBFNPatched(bfnConfig.game_path);

		ImGui::SetCursorPosX(centerOffset);
		ImGui::TextUnformatted("BFN Executable:");
		ImGui::SetCursorPosX(centerOffset);
		ImGui::SetNextItemWidth(fieldWidth - 180 * dpiScale);

		char buf[512];
		strncpy_s(buf, bfnConfig.game_path.c_str(), sizeof(buf));
		if (ImGui::InputText("##BFNExePath", buf, sizeof(buf))) {
			bfnConfig.game_path = buf;
			save_config(g_config, "config.json");
			if (!bfnConfig.game_path.empty()) isPatched = Patcher::IsBFNPatched(bfnConfig.game_path);
		}

		ImGui::SameLine();
		if (ImGui::Button("Auto Detect", ImVec2(100 * dpiScale, 0))) {
			std::string detected = Utils::Registry::GetGamePathFromRegistry(2);
			if (!detected.empty()) {
				fs::path exePath = fs::path(detected);
				exePath /= "PVZBattleforNeighborville.exe";
				bfnConfig.game_path = exePath.string();
				save_config(g_config, "config.json");
				showStatus("Auto detected BFN path: " + exePath.string());
				isPatched = Patcher::IsBFNPatched(bfnConfig.game_path);
			}
			else {
				showStatus("Could not auto detect BFN installation", true);
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Browse", ImVec2(80 * dpiScale, 0))) {
			std::string chosen = Utils::Dialog::BrowseForExe(hwnd, 2);
			if (!chosen.empty()) {
				bfnConfig.game_path = chosen;
				save_config(g_config, "config.json");
				showStatus("BFN path set: " + chosen);
				isPatched = Patcher::IsBFNPatched(bfnConfig.game_path);
			}
		}

		ImGui::Dummy(ImVec2(0, 20 * dpiScale));
		fs::path launcherDir = fs::path([] {
			char path[MAX_PATH];
			GetModuleFileNameA(NULL, path, MAX_PATH);
			return std::string(path);
			}()).parent_path();

		fs::path patchesDir = launcherDir / "patches" / "bfn";
		std::string dllFile = (launcherDir / "dinput8.dll").string();
		bool hasPatchFiles = fs::exists(patchesDir / "PVZBattleforNeighborville.exe.xdelta");
		bool hasDLL = fs::exists(dllFile);

		if (!hasPatchFiles || !hasDLL) {
			ImGui::SetCursorPosX(centerOffset);
			ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Missing required files:");
			if (!hasPatchFiles) {
				ImGui::SetCursorPosX(centerOffset);
				ImGui::Text("- BFN patch files in patches/bfn/");
			}
			if (!hasDLL) {
				ImGui::SetCursorPosX(centerOffset);
				ImGui::Text("- dinput8.dll");
			}
		}

		if (isPatched) {
			ImGui::SetCursorPosX(centerOffset);
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "BFN is already patched");
			if (Utils::UI::CenteredButton("RESTORE ORIGINAL BFN", 200 * dpiScale, dpiScale)) {
				std::string err;
				if (Patcher::RestoreBFN(bfnConfig.game_path, err)) {
					isPatched = false;
					showStatus("BFN restored to original");
				}
				else {
					showStatus("Error: " + err, true);
				}
			}
		}
		else {
			ImGui::SetCursorPosX(centerOffset);
			ImGui::Text("One click patch for Battle for Neighborville");
			ImGui::BeginDisabled(!hasPatchFiles || !hasDLL || bfnConfig.game_path.empty());
			if (Utils::UI::CenteredButton("AUTO PATCH BFN", 200 * dpiScale, dpiScale)) {
				std::string err;
				if (Patcher::AutoPatchBFN(bfnConfig.game_path, patchesDir.string(), dllFile, err)) {
					isPatched = true;
					showStatus("BFN patched successfully!");
				}
				else {
					showStatus("Error: " + err, true);
				}
			}
			ImGui::EndDisabled();
		}
	}

	drawStatusMessage(dpiScale);
}

void UI::DrawUI(HWND hwnd, float dpiScale) {
	static int currentTab = 0;

	setupStyle(dpiScale);

	RECT rc;
	GetClientRect(hwnd, &rc);
	ImVec2 size((float)(rc.right - rc.left), (float)(rc.bottom - rc.top));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(size);

	ImGui::Begin("Garden Gate", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	drawTitleBar(hwnd, dpiScale);
	drawTabBar(currentTab, dpiScale);

	ImGui::BeginChild("ContentArea", ImVec2(0, ImGui::GetContentRegionAvail().y), false);

	if (currentTab == 0) {
		drawLauncherTab(hwnd, dpiScale);
	}
	else {
		drawPatcherTab(hwnd, dpiScale);
	}

	ImGui::EndChild();
	ImGui::End();
}
