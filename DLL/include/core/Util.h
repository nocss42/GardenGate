#pragma once

#include <thread>
#include <windows.h>

template<std::size_t N>
struct Patch
{
    std::uintptr_t addr;
    std::array<std::uint8_t, N> bytes;
};

void WriteToMemory(intptr_t offset, char* value, size_t length)
{
    for (int i = 0; i < length; i++)
    {
        DWORD d, ds;

        VirtualProtect((LPVOID)(offset + i), 1, PAGE_EXECUTE_READWRITE, &d);
        *(char*)(offset + i) = value[i];
        VirtualProtect((LPVOID)(offset + i), 1, d, &ds);
    }
}

template<std::size_t N>
inline void ApplyPatch(const Patch<N>& p)
{
    WriteToMemory(p.addr, (char*)p.bytes.data(), p.bytes.size());
}