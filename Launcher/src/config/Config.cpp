#include "Config.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

void to_json(ordered_json& j, const GameConfig& config)
{
    j = ordered_json{ { "game_path", config.game_path }, { "custom_args", config.custom_args },
        { "moddata_enabled", config.moddata_enabled }, { "moddata_selected", config.moddata_selected } };
}

void from_json(const json& j, GameConfig& config)
{
    if (j.contains("game_path"))
        config.game_path = j["game_path"];
    if (j.contains("custom_args"))
        config.custom_args = j["custom_args"];
    if (j.contains("moddata_enabled"))
        config.moddata_enabled = j["moddata_enabled"];
    if (j.contains("moddata_selected"))
        config.moddata_selected = j["moddata_selected"];
}

void to_json(ordered_json& j, const Config& config)
{
    j = ordered_json{ { "username", config.username }, { "password", config.password }, { "server_ip", config.server_ip },

        { "game_selected", config.game_selected }, { "launch_directly", config.launch_directly },

        { "gw1", config.gw1 }, { "gw2", config.gw2 }, { "bfn", config.bfn },

        { "baseColor", { config.baseColor.x, config.baseColor.y, config.baseColor.z, config.baseColor.w } },
        { "baseHover", { config.baseHover.x, config.baseHover.y, config.baseHover.z, config.baseHover.w } },
        { "baseActive", { config.baseActive.x, config.baseActive.y, config.baseActive.z, config.baseActive.w } } };
}

void from_json(const json& j, Config& config)
{
    if (j.contains("username"))
        config.username = j["username"];
    if (j.contains("server_ip"))
        config.server_ip = j["server_ip"];
    if (j.contains("password"))
        config.password = j["password"];

    if (j.contains("launch_directly"))
        config.launch_directly = j["launch_directly"];
    if (j.contains("game_selected"))
        config.game_selected = j["game_selected"];

    if (j.contains("gw1"))
        from_json(j["gw1"], config.gw1);
    if (j.contains("gw2"))
        from_json(j["gw2"], config.gw2);
    if (j.contains("bfn"))
        from_json(j["bfn"], config.bfn);

    if (j.contains("baseColor"))
    {
        auto arr = j["baseColor"];
        if (arr.size() == 4)
            config.baseColor = ImVec4(arr[0], arr[1], arr[2], arr[3]);
    }
    if (j.contains("baseHover"))
    {
        auto arr = j["baseHover"];
        if (arr.size() == 4)
            config.baseHover = ImVec4(arr[0], arr[1], arr[2], arr[3]);
    }
    if (j.contains("baseActive"))
    {
        auto arr = j["baseActive"];
        if (arr.size() == 4)
            config.baseActive = ImVec4(arr[0], arr[1], arr[2], arr[3]);
    }
}

Config load_config(const std::string& filepath)
{
    Config config;
    std::ifstream file(filepath);
    if (file.is_open())
    {
        try
        {
            json j;
            file >> j;
            from_json(j, config);
        }
        catch (const json::exception& e)
        {
            std::cerr << "Error parsing config: " << e.what() << std::endl;
        }
    }
    return config;
}

void save_config(const Config& config, const std::string& filepath)
{
    std::ofstream file(filepath);
    if (file.is_open())
    {
        ordered_json j;
        to_json(j, config);
        file << j.dump(2);
    }
}