#pragma once
#include <cstddef>
#include <cstdint>
#include <stdexcept>

template <class T>
class Array final
{
public:
    using size_type = std::size_t;

    [[nodiscard]] std::int32_t size() const noexcept
    {
        return *reinterpret_cast<const std::int32_t *>(
            reinterpret_cast<const std::byte *>(m_data) - 4);
    }

    T &operator[](size_type i)
    {
        check(i);
        return m_data[i];
    }

    const T &operator[](size_type i) const
    {
        check(i);
        return m_data[i];
    }

    T *begin() noexcept { return m_data; }
    T *end() noexcept { return m_data + static_cast<size_type>(size()); }
    const T *begin() const noexcept { return m_data; }
    const T *end() const noexcept { return m_data + static_cast<size_type>(size()); }

private:
    void check(size_type i) const
    {
        const auto n = size();
        if (n < 0 || i >= static_cast<size_type>(n))
            throw std::out_of_range("Array index out of bounds.");
    }

    T *m_data = nullptr;
};
