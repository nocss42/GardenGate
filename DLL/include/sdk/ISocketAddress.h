#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <string_view>

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

class ISocketAddress final
{
public:
    ISocketAddress() noexcept
    {
        clear();
    }

    ISocketAddress(const ISocketAddress& other) noexcept = default;
    ISocketAddress& operator=(const ISocketAddress& other) noexcept = default;

    ISocketAddress(std::string_view ipv4, std::uint16_t port) noexcept
    {
        clear();

        sockaddr_in a{};
        a.sin_family = AF_INET;
        a.sin_port = ::htons(port);
        ::inet_pton(AF_INET, ipv4.data(), &a.sin_addr);

        set_data(&a, static_cast<int>(sizeof(a)));
    }

    void clear() noexcept
    {
        m_length = 0;
        m_address.fill(0);
    }

    void set_data(const void* data, int len) noexcept
    {
        if (!data || len <= 0)
        {
            clear();
            return;
        }

        const auto n = (len > static_cast<int>(m_address.size())) ? static_cast<int>(m_address.size()) : len;

        std::memcpy(m_address.data(), data, static_cast<std::size_t>(n));
        m_length = static_cast<std::uint8_t>(n);
    }

    [[nodiscard]] const std::uint8_t* data() const noexcept
    {
        return m_address.data();
    }
    [[nodiscard]] int length() const noexcept
    {
        return m_length;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        for (int i = 0; i < m_length; ++i)
            if (m_address[static_cast<std::size_t>(i)] != 0)
                return false;
        return true;
    }

    explicit operator bool() const noexcept
    {
        return !empty();
    }

    [[nodiscard]] bool operator==(const ISocketAddress& rhs) const noexcept
    {
        if (m_length != rhs.m_length)
            return false;
        return std::memcmp(m_address.data(), rhs.m_address.data(), m_length) == 0;
    }

    [[nodiscard]] bool operator!=(const ISocketAddress& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    [[nodiscard]] bool operator<(const ISocketAddress& rhs) const noexcept
    {
        if (m_length != rhs.m_length)
            return m_length < rhs.m_length;
        return std::memcmp(m_address.data(), rhs.m_address.data(), m_length) < 0;
    }

    [[nodiscard]] std::uint16_t get_port() const noexcept
    {
        if (m_length < static_cast<std::uint8_t>(sizeof(sockaddr_in)))
            return 0;
        const auto* a = reinterpret_cast<const sockaddr_in*>(m_address.data());
        return ::ntohs(a->sin_port);
    }

    [[nodiscard]] bool is_broadcasting() const noexcept
    {
        if (m_length < static_cast<std::uint8_t>(sizeof(sockaddr_in)))
            return false;
        const auto* a = reinterpret_cast<const sockaddr_in*>(m_address.data());
        return a->sin_family == AF_INET && a->sin_addr.s_addr == INADDR_BROADCAST;
    }

    [[nodiscard]] const char* get_address() const noexcept
    {
        thread_local std::array<char, INET_ADDRSTRLEN> buf{};
        if (m_length < static_cast<std::uint8_t>(sizeof(sockaddr_in)))
            return "";

        const auto* a = reinterpret_cast<const sockaddr_in*>(m_address.data());
        if (!::inet_ntop(AF_INET, const_cast<IN_ADDR*>(&a->sin_addr), buf.data(), static_cast<DWORD>(buf.size())))
            return "";

        return buf.data();
    }

private:
    std::array<std::uint8_t, 64> m_address{};
    std::uint8_t m_length{ 0 };
};
