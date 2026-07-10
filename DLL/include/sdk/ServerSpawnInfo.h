#pragma once

#include "sdk/LevelSetup.h"

namespace fb
{
namespace gw1
{
struct ServerSpawnInfo
{
    struct SaveGameData : public FBArray<uint8_t>
    {};

    ServerSpawnInfo(LevelSetup& setup)
        : levelSetup(setup)
    {}

    void* fileSystem = nullptr;
    void* damageArbitrator = nullptr;
    void* playerManager = nullptr;
    LevelSetup& levelSetup;
    unsigned int tickFrequency = 0;
    bool isSinglePlayer = false;
    bool isLocalHost = false;
    bool isDedicated = false;
    bool isEncrypted = false;
    bool isCoop = false;
    bool isMenu = false;
    bool keepResources = false;
    SaveGameData saveData;
    void* serverCallbacks = nullptr;
    void* runtimeModules = nullptr;
    mutable void* loadInfo = nullptr;
    unsigned int serverPort = 0;
    unsigned int validLocalPlayersMask = 1;
};
} // namespace gw1

namespace gw2
{
struct ServerSpawnInfo
{
    struct SaveGameData : public FBArray<uint8_t>
    {};

    ServerSpawnInfo(LevelSetup& setup)
        : levelSetup(setup)
    {}

    void* fileSystem = nullptr;
    void* damageArbitrator = nullptr;
    void* playerManager = nullptr;
    LevelSetup& levelSetup;
    unsigned int tickFrequency = 0;
    bool isSinglePlayer = false;
    bool isLocalHost = false;
    bool isDedicated = false;
    bool isEncrypted = false;
    bool isCoop = false;
    bool isMenu = false;
    bool keepResources = false;
    SaveGameData saveData;
    void* serverCallbacks = nullptr;
    void* runtimeModules = nullptr;
    mutable void* loadInfo = nullptr;
    unsigned int serverPort = 0;
    unsigned int validLocalPlayersMask = 1;
};
} // namespace gw2

namespace gw3
{
struct ServerSpawnInfo
{
    struct SaveGameData : public FBArray<uint8_t>
    {};

    ServerSpawnInfo(LevelSetup& setup)
        : levelSetup(setup)
    {}

    void* fileSystem = nullptr;
    void* damageArbitrator = nullptr;
    void* playerManager = nullptr;
    LevelSetup& levelSetup;
    unsigned int tickFrequency = 30;
    bool isSinglePlayer = false;
    bool isLocalHost = false;
    bool isDedicated = false;
    bool isEncrypted = false;
    bool isCoop = false;
    bool isMenu = false;
    bool keepResources = false;
    SaveGameData saveData;
    void* serverCallbacks = nullptr;
    void* runtimeModules = nullptr;
    void* idk = nullptr;
    mutable void* loadInfo = nullptr;
    unsigned int serverPort = 0;
    unsigned int validLocalPlayersMask = 1;
};
} // namespace gw3
} // namespace fb
