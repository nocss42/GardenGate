#pragma once

#include "Array.h"
#include "LevelSetupOption.h"
#include "Types.h"

namespace fb
{
namespace gw1
{
struct LevelSetup
{
    String Name;
    Array<LevelSetupOption> InclusionOptions;
    int32_t DifficultyIndex;
    Array<String> SubLevelNames;
    String StartPoint;
    Array<int32_t> SubLevelStates;
    bool IsSaveGame;
    bool ForceReloadResources;
};
} // namespace gw1

namespace gw2
{
struct LevelSetup
{
    String Name;
    Array<LevelSetupOption> InclusionOptions;
    int32_t DifficultyIndex;
    String StartPoint;
    String Playlist;
    String LoadScreen_GameMode;
    String LoadScreen_LevelName;
    String LoadScreen_LevelDescription;
    String LoadScreen_LevelAsset;
    bool IsSaveGame;
    bool HasPersistentSave;
    bool ForceReloadResources;
};
} // namespace gw2

namespace gw3
{
enum LoadScreenLevelCategory
{
    LoadScreenLevelCategory_None = 0,
    LoadScreenLevelCategory_Social = 1,
    LoadScreenLevelCategory_Sandbox = 2,
    LoadScreenLevelCategory_PvP = 3,
    LoadScreenLevelCategory_Ops = 4
};

struct LevelSetup
{
    Array<LevelSetupOption> InclusionOptions;
    u32 DifficultyIndex;
    String LoadScreen_GameMode;
    String StartPoint;
    String LevelManagerInitialLevel;
    String LoadScreen_LevelName;
    String Name;
    String LevelManagerStartPoint;
    String LoadScreen_LevelAsset;
    String Playlist;
    String LoadScreen_LevelDescription;
    LoadScreenLevelCategory LoadScreen_LevelCategory;
    bool HasPersistentSave;
    bool ForceReloadResources;
    bool IsSaveGame;
};
} // namespace gw3
} // namespace fb
