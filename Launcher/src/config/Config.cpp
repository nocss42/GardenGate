#include "Config.hpp"

#include <fstream>

using json = nlohmann::json;

void GameConfig::from_json(const json& j) {
    game_path = j.value("game_path", "");
    custom_args = j.value("custom_args", "");
    moddata_enabled = j.value("moddata_enabled", true);
    moddata_selected = j.value("moddata_selected", "Default");

    moddata_items.clear();
    if (j.contains("moddata_items") && j["moddata_items"].is_array()) {
        for (const auto& item : j["moddata_items"]) {
            moddata_items.push_back(item.get<std::string>());
        }
    }
}

json GameConfig::to_json() const {
    return {
        {"game_path", game_path},
        {"custom_args", custom_args},
        {"moddata_enabled", moddata_enabled},
        {"moddata_selected", moddata_selected},
        {"moddata_items", moddata_items}
    };
}

Config load_config(const std::string& file) {
    Config cfg;
    std::ifstream in(file);
    if (!in.is_open()) return cfg;

    json j; in >> j;

    cfg.set_username(j.value("username", "").c_str());
    cfg.server_ip = j.value("server_ip", "");
    cfg.password = j.value("password", "");
    cfg.game_selected = j.value("game_selected", "GW1");

    if (j.contains("gw1")) cfg.gw1.from_json(j["gw1"]);
    if (j.contains("gw2")) cfg.gw2.from_json(j["gw2"]);
    if (j.contains("bfn")) cfg.bfn.from_json(j["bfn"]);

    if (cfg.gw1.game_path.empty() && j.contains("game_path"))
        cfg.gw1.game_path = j["game_path"].get<std::string>();
    if (cfg.gw2.game_path.empty() && j.contains("game_path"))
        cfg.gw2.game_path = j["game_path"].get<std::string>();
    if (cfg.bfn.game_path.empty() && j.contains("game_path"))
        cfg.bfn.game_path = j["game_path"].get<std::string>();

    return cfg;
}

void save_config(const Config& cfg, const std::string& file) {
    json j = {
        {"username", cfg.username},
        {"server_ip", cfg.server_ip},
        {"password", cfg.password},
        {"game_selected", cfg.game_selected},
        {"gw1", cfg.gw1.to_json()},
        {"gw2", cfg.gw2.to_json()},
        {"bfn", cfg.bfn.to_json()}
    };

    std::ofstream(file) << j.dump(4);
}
