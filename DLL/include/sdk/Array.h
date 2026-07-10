#pragma once
#include <cstddef>
#include <cstdint>
#include <stdexcept>

template<class T>
class Array final
{
public:
    using size_type = std::size_t;

    [[nodiscard]] std::int32_t size() const noexcept
    {
        return *reinterpret_cast<const std::int32_t*>(reinterpret_cast<const std::byte*>(m_data) - 4);
    }

    T& operator[](size_type i)
    {
        check(i);
        return m_data[i];
    }

    const T& operator[](size_type i) const
    {
        check(i);
        return m_data[i];
    }

    T* begin() noexcept
    {
        return m_data;
    }
    T* end() noexcept
    {
        return m_data + static_cast<size_type>(size());
    }
    const T* begin() const noexcept
    {
        return m_data;
    }
    const T* end() const noexcept
    {
        return m_data + static_cast<size_type>(size());
    }

    void set(T* ptr)
    {
        m_data = ptr;
    }

    void init(std::size_t capacity)
    {
        const std::size_t bytes = 4 + sizeof(T) * capacity;

        auto* base = static_cast<std::byte*>(::operator new(bytes, std::nothrow));
        if (!base)
        {
            m_data = nullptr;
            return;
        }

        *reinterpret_cast<std::int32_t*>(base) = 0;
        m_data = reinterpret_cast<T*>(base + 4);
    }

private:
    void check(size_type i) const
    {
        const auto n = size();
        if (n < 0 || i >= static_cast<size_type>(n))
            throw std::out_of_range("Array index out of bounds.");
    }

    T* m_data = nullptr;
};

struct ArrayBase
{
    static constexpr uint32_t __declspec(align(16)) g_emptyArray[8] = {};
    void* emptyArrayBegin()
    {
        return (void*)&(g_emptyArray[4]);
    }
};

template<typename T>
class FBArray : public ArrayBase
{
public:
    T* m_data = nullptr;

    void reset()
    {
        m_data = (T*)emptyArrayBegin();
    }

    inline void init(uint32_t size)
    {
        if (size == 0)
        {
            reset();
            return;
        }
    }

    FBArray()
    {
        reset();
    }
};

template<class T>
struct List
{
    T* data;
    int count;
    int capacity;
};
