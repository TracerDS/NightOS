#pragma once

#include <cstdint>
#include <io.hpp>

namespace StackFrame {
    struct Frame {
        Frame* ebp;
        std::uint32_t eip;
    };

    inline void PrintFrames(int maxFrames) {
        Frame* currentFrame = reinterpret_cast<Frame*>(__builtin_frame_address(0));

        IO::kprintf("Stack trace:\r\n");
        for (auto i=0; currentFrame && i < maxFrames; ++i) {
            IO::kprintf(
                "- Frame %d: EBP=0x%08x, EIP=0x%08x\r\n",
                i, 
                reinterpret_cast<std::uintptr_t>(currentFrame->ebp), 
                currentFrame->eip
            );
            currentFrame = currentFrame->ebp;
        }
    }
}