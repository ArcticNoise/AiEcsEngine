#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <type_traits>

namespace x2d {

template <class T, std::size_t Capacity = 256> class EventQueue final {
  public:
    static_assert(std::is_trivial_v<T>, "Event must be POD");

    bool Enqueue(const T &event) {
        const std::size_t head = m_Head.load(std::memory_order_relaxed);
        const std::size_t next = (head + 1) % Capacity;
        if (next == m_Tail.load(std::memory_order_acquire)) {
            return false;
        }
        m_Buffer[head] = event;
        m_Head.store(next, std::memory_order_release);
        return true;
    }

    bool Dequeue(T &outEvent) {
        const std::size_t tail = m_Tail.load(std::memory_order_relaxed);
        if (tail == m_Head.load(std::memory_order_acquire)) {
            return false;
        }
        outEvent = m_Buffer[tail];
        m_Tail.store((tail + 1) % Capacity, std::memory_order_release);
        return true;
    }

    bool IsEmpty() const {
        return m_Head.load(std::memory_order_acquire) == m_Tail.load(std::memory_order_acquire);
    }

  private:
    std::array<T, Capacity> m_Buffer{};
    std::atomic<std::size_t> m_Head{0};
    std::atomic<std::size_t> m_Tail{0};
};

} // namespace x2d
