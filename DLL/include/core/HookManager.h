#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include <unordered_map>
#include <span>

#include <MinHook.h>

#include "core/Hook.h"
#include "base/Log.h"

namespace GG
{
struct HookTemplate
{
    std::uintptr_t target;
    void* detour;
    bool enable = true;
};

class HookManager final
{
public:
    HookManager() = default;

    HookManager(const HookManager&) = delete;
    HookManager& operator=(const HookManager&) = delete;

    ~HookManager() noexcept
    {
        if (m_initialized)
            uninitialize();
    }

    static HookManager& getManager() noexcept
    {
        static HookManager s;
        return s;
    }

    void initialize() noexcept
    {
        if (m_initialized)
            return;

        const auto st = ::MH_Initialize();
        if (st != MH_OK)
            GG_FATAL("MH_Initialize failed (%d).", (int)st);

        m_initialized = true;
    }

    void uninitialize() noexcept
    {
        if (!m_initialized)
            return;

        disableAll();
        uninstallAll();
        m_entries.clear();
        m_detourToOriginal.clear();

        const auto st = ::MH_Uninitialize();
        if (st != MH_OK)
            GG_FATAL("MH_Uninitialize failed (%d).", (int)st);

        m_initialized = false;
    }

    void registerHooks(std::span<const HookTemplate> templates) noexcept
    {
        if (!m_initialized)
            initialize();

        for (const auto& s : templates)
            registerHook(s.target, s.detour, s.enable);
    }

    void registerHook(std::uintptr_t target, void* detour, bool enable_now = true) noexcept
    {
        if (!m_initialized)
            initialize();
        if (!detour || !target)
            GG_FATAL("registerHook called with null target/detour.");

        auto e = std::make_unique<Entry>();
        e->detour = detour;

        e->hook = Hook{ target, detour, reinterpret_cast<void**>(&e->original) };
        e->hook.install();
        if (enable_now)
            e->hook.enable();

        m_detourToOriginal.emplace(detour, &e->original);
        m_entries.emplace_back(std::move(e));
    }

    void enableAll() noexcept
    {
        for (auto& e : m_entries)
            e->hook.enable();
    }

    void disableAll() noexcept
    {
        for (auto it = m_entries.rbegin(); it != m_entries.rend(); ++it)
            (*it)->hook.disable();
    }

    void uninstallAll() noexcept
    {
        for (auto it = m_entries.rbegin(); it != m_entries.rend(); ++it)
            (*it)->hook.reset();
    }

    template<typename Fn>
    [[nodiscard]] Fn Call(Fn detour_fn) const noexcept
    {
        const auto it = m_detourToOriginal.find(reinterpret_cast<void*>(detour_fn));
        if (it == m_detourToOriginal.end() || !it->second || !*it->second)
            GG_FATAL("HookManager::Call failed (detour not registered or original missing).");
        return reinterpret_cast<Fn>(*it->second);
    }

    static void* HookVTableFunction(void* pVtable, void* fnHookFunc, int offset)
    {
        intptr_t vtable = *((intptr_t*)pVtable);
        intptr_t func = vtable + sizeof(intptr_t) * offset;
        intptr_t orig = *((intptr_t*)func);

        GG_LOG(GG::LogLevel::Debug, "HookVTableFunction - vt: %p, func: %p, orig: %p)", vtable, func, orig);

        MEMORY_BASIC_INFORMATION mbi;
        VirtualQuery((LPCVOID)func, &mbi, sizeof(mbi));
        VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);

        *((intptr_t*)func) = (intptr_t)fnHookFunc;

        VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);

        return reinterpret_cast<void*>(orig);
    }

private:
    struct Entry
    {
        Hook hook{};
        void* detour = nullptr;
        void* original = nullptr;
    };

    std::vector<std::unique_ptr<Entry>> m_entries;
    std::unordered_map<void*, void**> m_detourToOriginal;
    bool m_initialized = false;
};
} // namespace GG
