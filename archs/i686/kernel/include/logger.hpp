#pragma once

namespace Log {
    class Logger {
    public:
        static void log(const char* message);
        static void log(const char* fmt, ...);
    };
}