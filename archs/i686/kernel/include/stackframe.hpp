#pragma once

#include <init.hpp>

#include <cstdint>

namespace NOS::StackFrame {
    struct Frame {
        Frame* ebp;
        std::uint32_t eip;
    };

    void PrintFrames(std::uint32_t maxFrames) noexcept;
}