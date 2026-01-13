#pragma once

namespace NOS::Logger {
    void Log(const char* fmt, ...) noexcept;
    
    void LogError(const char* fmt, ...) noexcept;
}