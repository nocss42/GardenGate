#pragma once

#include "LevelSetup.h"
#include "ISocketManager.h"

namespace fb
{
struct ServerSpawnOverrides
{
    void* levelSetup;
    ISocketManager* socketManager;
    intptr_t connectionCreator;
    intptr_t peerCreator;
};
} // namespace fb