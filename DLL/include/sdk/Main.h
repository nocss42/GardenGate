#pragma once

#include "sdk/Types.h"

namespace fb
{
struct Main
{
    intptr_t __vftable;
    char __pad[24];
    bool m_isDedicatedServer;
    bool m_isDedicatedServerData;
};
} // namespace fb