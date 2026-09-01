#pragma once

#include <klibc/format>

namespace NOS::Logger {
    void Log(const char* fmt, ...) noexcept;    
    void LogError(const char* fmt, ...) noexcept;

    // template <class... Args>
    // void Log(klibc::format_string<Args...> fmt, Args&&... args) noexcept;

    // template <class... Args>
    // void LogError(klibc::format_string<Args...> fmt, Args&&... args) noexcept;
}