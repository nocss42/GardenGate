#pragma once

namespace eastl {

    template<typename T>
    class vector {
    private:
        T* m_begin;
        T* m_end;
        T* m_capacity;
    public:

        size_t size() const { return m_end - m_begin; }
        size_t capacity() const { return m_capacity - m_begin; }

        bool empty() const { return !m_begin || m_begin == m_end;  }

        T* begin() { return m_begin; }
        T* end() { return m_begin + size(); }

        T& operator[](int idx) { return m_begin[idx]; }

        T& at(int idx)
        {
            if (idx < size()) {
                return m_begin[idx];
            }
            throw std::exception("Outside of the range");
        }
    };
}