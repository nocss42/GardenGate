#pragma once
#include <string>
#include <vector>
#include <imgui.h>

#include <nlohmann/json.hpp>

struct GameConfig
{
    std::string game_path;
    std::string custom_args;
    bool moddata_enabled = false; // Mods are no longer needed by default
    std::vector<std::string> moddata_items;
    std::string moddata_selected = "Default";

    void from_json(const nlohmann::json& j);
    nlohmann::json to_json() const;
};

struct Config
{
    std::string username;
    std::string server_ip;
    std::string password;

    std::string game_selected = "gw1";
    std::string patch_selected = "gw1";
    bool launch_directly = false;
    bool log_level_enabled = false;
    std::string log_level = "Info";

    ImVec4 baseColor = ImVec4(0.25f, 0.47f, 0.32f, 1.0f);
    ImVec4 baseHover = ImVec4(0.30f, 0.55f, 0.38f, 1.0f);
    ImVec4 baseActive = ImVec4(0.20f, 0.40f, 0.28f, 1.0f);

    GameConfig gw1;
    GameConfig gw2;
    GameConfig bfn;

    void set_username(const char* val)
    {
        username = val;
        if (username.length() > 15)
            username.resize(15);
    }

    GameConfig& get_current_game()
    {
        if (game_selected == "gw2")
            return gw2;
        if (game_selected == "bfn")
            return bfn;
        return gw1;
    }

    const GameConfig& get_current_game() const
    {
        if (game_selected == "gw2")
            return gw2;
        if (game_selected == "bfn")
            return bfn;
        return gw1;
    }

    int get_game_selected_int() const
    {
        if (game_selected == "gw2")
            return 1;
        if (game_selected == "bfn")
            return 2;
        return 0;
    }

    void set_game_selected_from_int(int val)
    {
        if (val == 1)
            game_selected = "gw2";
        else if (val == 2)
            game_selected = "bfn";
        else
            game_selected = "gw1";
    }

    int get_patch_selected_int() const {
        if (patch_selected == "gw2")
        	return 1;
        if (patch_selected == "bfn")
        	return 2;
        return 0;
    }

    void set_patch_selected_from_int(int val) {
        if (val == 1) patch_selected = "gw2";
        else if (val == 2) patch_selected = "bfn";
        else patch_selected = "gw1";
    }

    void set_log_level_from_int(int val)
    {
        switch (val)
        {
        case 0: log_level = "DebugPlusPlus"; break;
        case 1: log_level = "Debug"; break;
        case 2: log_level = "Info"; break;
        case 3: log_level = "Warning"; break;
        case 4: log_level = "Error"; break;
        case 5: log_level = "Fatal"; break;
        default: log_level = "Info"; break;
        }
    }

    int get_log_level_int() const
    {
        if (log_level == "DebugPlusPlus") return 0;
        if (log_level == "Debug") return 1;
        if (log_level == "Info") return 2;
        if (log_level == "Warning") return 3;
        if (log_level == "Error") return 4;
        if (log_level == "Fatal") return 5;
        return 2;
    }
};

Config load_config(const std::string& file);
void save_config(const Config& cfg, const std::string& file);
void save_config(const Config& cfg, const std::string& file);
