// Copyright BattleDash. All Rights Reserved.

#pragma once

#include <SDK/Levels.h>

#include <string>
#include <vector>

namespace Kyber
{
struct GameMode
{
    const char* mode;
    const char* name;
    std::vector<const char*> levels;
    std::vector<GameLevel> levelOverrides;
};

//clang-format off
// What if you wanted to format something nicely, but clang-format said:
static GameMode s_game_modes[] = { 
    { "Coop0", "CO-OP TEST",
    {
        "_pvz/Levels/Coastal/Level_COOP_Coastal/Level_COOP_Coastal" // level path
    },
    { {
        "_pvz/Levels/Coastal/Level_COOP_Coastal/Level_COOP_Coastal",
        "SHARKBITE SHORES" // display name override
    } } } };


GameMode GetGameMode(const char* mode);
GameLevel GetGameLevel(GameMode mode, const char* level);
} // namespace Kyber