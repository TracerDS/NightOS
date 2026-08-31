#pragma once

#include <core/init.hpp>
#include <atomic>

namespace NOS::Memory::Lock {
    class Spinlock {
    public:
        constexpr Spinlock() noexcept = default;

        void lock() noexcept;
        void unlock() noexcept;
        bool try_lock() noexcept;
    private:
        std::atomic_flag m_lock = ATOMIC_FLAG_INIT;
    };

    class [[nodiscard]] LockGuard {
    public:
        explicit LockGuard(Spinlock& lock) noexcept;
        ~LockGuard() noexcept;

        LockGuard(const LockGuard&) = delete;
        LockGuard& operator=(const LockGuard&) = delete;
    private:
        Spinlock& m_lock;
    };
}