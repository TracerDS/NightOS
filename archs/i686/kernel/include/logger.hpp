#pragma once

namespace NOS::Logger {
    void Log(const char* message);
    void Log(const char* fmt, ...);
    
    void LogError(const char* message);
    void LogError(const char* fmt, ...);
}