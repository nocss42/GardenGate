#pragma once
#include <cstdint>
#include <utility>

#include <MinHook.h>

#include "base/Log.h"

namespace GG
{
class Hook final
{
public:
    Hook() = default;

    Hook(std::uintptr_t target, void* detour, void** original_out) noexcept
        : m_target(reinterpret_cast<void*>(target))
        , m_detour(detour)
        , m_original_out(original_out)
    {}

    Hook(void* target, void* detour, void** original_out) noexcept
        : m_target(target)
        , m_detour(detour)
        , m_original_out(original_out)
    {}

    Hook(const Hook&) = delete;
    Hook& operator=(const Hook&) = delete;

    Hook(Hook&& other) noexcept
    {
        *this = std::move(other);
    }
    Hook& operator=(Hook&& other) noexcept
    {
        if (this == &other)
            return *this;
        reset();

        m_target = std::exchange(other.m_target, nullptr);
        m_detour = std::exchange(other.m_detour, nullptr);
        m_original_out = std::exchange(other.m_original_out, nullptr);
        m_state = std::exchange(other.m_state, State::Empty);

        return *this;
    }

    ~Hook() noexcept
    {
        reset();
    }

    [[nodiscard]] bool valid() const noexcept
    {
        return m_target && m_detour && m_original_out;
    }

    void install() noexcept
    {
        if (!valid())
            GG_FATAL("Hook::install called with invalid hook parameters.");
        if (m_state != State::Empty)
            return;

        const auto st = ::MH_CreateHook(m_target, m_detour, m_original_out);
        if (st != MH_OK)
            GG_FATAL("MH_CreateHook failed (%d).", (int)st);

        m_state = State::Created;
    }

    void enable() noexcept
    {
        if (!valid())
            GG_FATAL("Hook::enable called with invalid hook parameters.");
        if (m_state == State::Empty)
            install();
        if (m_state == State::Enabled)
            return;

        const auto st = ::MH_EnableHook(m_target);
        if (st != MH_OK)
            GG_FATAL("MH_EnableHook failed (%d).", (int)st);

        m_state = State::Enabled;
    }

    void disable() noexcept
    {
        if (m_state != State::Enabled)
            return;

        const auto st = ::MH_DisableHook(m_target);
        if (st != MH_OK)
            GG_FATAL("MH_DisableHook failed (%d).", (int)st);

        m_state = State::Created;
    }

    void reset() noexcept
    {
        if (m_state == State::Enabled)
            disable();

        if (m_state == State::Created)
        {
            const auto st = ::MH_RemoveHook(m_target);
            if (st != MH_OK)
                GG_FATAL("MH_RemoveHook failed (%d).", (int)st);
        }

        m_target = nullptr;
        m_detour = nullptr;
        m_original_out = nullptr;
        m_state = State::Empty;
    }

    [[nodiscard]] void* target() const noexcept
    {
        return m_target;
    }
    [[nodiscard]] void* detour() const noexcept
    {
        return m_detour;
    }

    template<class Fn>
    [[nodiscard]] Fn original() const noexcept
    {
        return (m_original_out && *m_original_out) ? reinterpret_cast<Fn>(*m_original_out) : nullptr;
    }

private:
    enum class State : unsigned char
    {
        Empty,
        Created,
        Enabled
    };

    void* m_target = nullptr;
    void* m_detour = nullptr;
    void** m_original_out = nullptr;
    State m_state = State::Empty;
};
} // namespace GG
