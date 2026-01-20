#include <stackframe.hpp>
#include <io.hpp>

namespace NOS::StackFrame {
    void PrintFrames(std::uint32_t maxFrames) noexcept {
        Frame* currentFrame = reinterpret_cast<Frame*>(__builtin_frame_address(0));

        IO::kprintf("Stack trace:\r\n");
        for (std::uint32_t i = 0; currentFrame && i < maxFrames; ++i) {
            IO::kprintf(
                "- Frame %ld: EBP=0x%08lx, EIP=0x%08lx\r\n",
                i,
                reinterpret_cast<std::uintptr_t>(currentFrame->ebp), 
                currentFrame->eip
            );
            currentFrame = currentFrame->ebp;
        }
    }
}