#include <descriptors/isr.hpp>
#include <descriptors/idt.hpp>
#include <descriptors/gdt.hpp>
#include <descriptors/descriptors.hpp>
#include <descriptors/irq.hpp>
#include <klibc/array>
#include <io.hpp>
#include <interfaces/streams/serial.hpp>

#include <utility>

namespace NOS {
    namespace Descriptors::GDT {
        extern std::uint8_t g_codeSegmentOffset;
        extern std::uint8_t g_dataSegmentOffset;
        extern std::uint8_t g_tssSegmentOffset;
    }

    namespace Interrupts::ISR {
        extern "C" void __kernel_serial_write_byte__(std::uint16_t port, std::uint8_t data);
        extern "C" std::uint32_t __kernel_get_cr2__() noexcept;

        extern "C" std::uint8_t __isr_stubs_start__[];
        extern "C" std::uint8_t __isr_stubs_end__[];

        constexpr bool __isr_is_error__(std::uint8_t interrupt) noexcept {
            switch (interrupt) {
                case 8:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 17:
                case 21:
                    return true;
                default:
                    return false;
            }
        }

        constexpr std::uint8_t __isr_get_alignment__(std::uint8_t interrupt) noexcept {
            return __isr_is_error__(interrupt) ? 7 : 9;
        }

        constexpr klibc::array<const char*, Descriptors::INTERRUPT_COUNT> g_ISRExceptions = {
            "Divide by zero error",           // 0
            "Debug exception",                // 1
            "Non-maskable Interrupt",         // 2
            "Breakpoint",                     // 3
            "Overflow",                       // 4
            "Bound Range Exceeded",           // 5
            "Invalid Opcode",                 // 6
            "Device Not Available",           // 7
            "Double Fault",                   // 8
            "Coprocessor Segment Overrun",    // 9
            "Invalid TSS",                    // 10
            "Segment Not Present",            // 11
            "Stack-Segment Fault",            // 12
            "General Protection Fault",       // 13
            "Page Fault",                     // 14
            "",                               // 15
            "x87 Floating-Point Exception",   // 16
            "Alignment Check",                // 17
            "Machine Check",                  // 18
            "SIMD Floating-Point Exception",  // 19
            "Virtualization Exception",       // 20
            "Control Protection Exception",   // 21
            "",                               // 22
            "",                               // 23
            "",                               // 24
            "",                               // 25
            "",                               // 26
            "",                               // 27
            "Hypervisor Injection Exception", // 28
            "VMM Communication Exception",    // 29
            "Security Exception",             // 30
            "",                               // 31
            0,                                // 32
            0,                                // 33
            0,                                // 34
            0,                                // 35
            0,                                // 36
            0,                                // 37
            0,                                // 38
            0,                                // 39
            0,                                // 40
            "",                               // 41
            0,                                // 42
            0,                                // 43
            0,                                // 44
            0,                                // 45
            0,                                // 46
            0,                                // 47
            0,                                // 48
            0,                                // 49
            "Kernel Panic",                   // 50
        };
        klibc::array<ISRHandler, Descriptors::INTERRUPT_COUNT> m_handlers;

        void RegisterHandler(std::uint8_t interrupt, ISRHandler handler) noexcept {
            m_handlers[interrupt] = handler;
        }

        void UnregisterHandler(std::uint8_t interrupt) noexcept {
            m_handlers[interrupt] = nullptr;
        }

        bool HasHandler(std::uint8_t interrupt) noexcept {
            return m_handlers[interrupt] != nullptr;
        }

        void DefaultHandler(InterruptState* regs) noexcept {
            const char* errMsg = g_ISRExceptions.size() > regs->interrupt
                ? g_ISRExceptions[regs->interrupt]
                : "Unknown Exception";

            IO::kprintf_color(
                "\r\nISR Handler %lu called!\r\n"
                "Exception: %s\r\n"
                "Code: 0x%08lX\r\n"
                "Registers:\r\n"
                "  EAX    = 0x%08lX\r\n"
                "  EBX    = 0x%08lX\r\n"
                "  ECX    = 0x%08lX\r\n"
                "  EDX    = 0x%08lX\r\n"
                "  ESI    = 0x%08lX\r\n"
                "  EDI    = 0x%08lX\r\n"
                "  EBP    = 0x%08lX\r\n"
                "  ESP    = 0x%08lX\r\n"
                "  CS     = 0x%08lX\r\n"
                "  DS     = 0x%08lX\r\n"
                "  ES     = 0x%08lX\r\n"
                "  FS     = 0x%08lX\r\n"
                "  GS     = 0x%08lX\r\n"
                "  SS     = 0x%08lX\r\n"
                "  EIP    = 0x%08lX\r\n"
                "  EFLAGS = 0x%08lX\r\n",
                Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
                Terminal::VGAColor::VGA_COLOR_BLACK,
                regs->interrupt,
                errMsg,
                regs->error,
                regs->eax,
                regs->ebx,
                regs->ecx,
                regs->edx,
                regs->esi,
                regs->edi,
                regs->ebp,
                regs->esp,
                regs->cs,
                regs->ds,
                regs->es,
                regs->fs,
                regs->gs,
                regs->ss,
                regs->eip,
                regs->eflags
            );
        }

        ISRHandler GetHandler(std::uint8_t interrupt) noexcept {
            if (!HasHandler(interrupt)) {
                return DefaultHandler;
            }
            return m_handlers[interrupt];
        }

        extern "C" void ISR_Handler(InterruptState* regs) {
            auto handler = GetHandler(regs->interrupt);

            if (!HasHandler(regs->interrupt)) {
                // Page fault?
                if (regs->interrupt == 0x0E) {
                    std::uint32_t faultAddr = __kernel_get_cr2__();

                    bool present = !(regs->error & 0x1); // 1 = access violation, 0 = page not present
                    bool rw = regs->error & 0x2;         // 1 = write, 0 = read
                    bool us = regs->error & 0x4;         // 1 = user mode, 0 = kernel mode
                    [[maybe_unused]] bool reserved = regs->error & 0x8;   // 1 = reserved bits overwritten
                    [[maybe_unused]] bool id = regs->error & 0x10;        // 1 = error during instruction fetch

                    IO::kprintf_color(
                        "\r\n[CRITICAL] PAGE FAULT at 0x%08lX\r\n"
                        "  Reason: %s while %s in %s\r\n"
                        "  Faulting address: 0x%08lX\r\n",
                        Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
                        Terminal::VGAColor::VGA_COLOR_BLACK,
                        regs->eip,
                        present ? "Protection Violation" : "Page Not Present",
                        rw ? "Writing" : "reading",
                        us ? "User mode" : "Kernel mode",
                        faultAddr
                    );
                    __asm__ volatile("hlt");
                    return;
                }
            }
            handler(regs);

            if (regs->interrupt >= 0x20 && regs->interrupt <= 0x2F) {
                if (regs->interrupt >= 0x28) {
                    // Send EOI to slave PIC
                    __kernel_serial_write_byte__(PIC::Ports::PIC2, PIC::Flags::PIC_EOI);
                }
                // Send EOI to master PIC
                __kernel_serial_write_byte__(PIC::Ports::PIC1, PIC::Flags::PIC_EOI);
                return;
            }
            __asm__ volatile("hlt");
        }
        
        void Init() noexcept {
            constexpr auto commonFlags =
                std::to_underlying(IDT::GateType::GATE_INT32) |
                std::to_underlying(Descriptors::PrivilegeType::PT_KERNEL);

            std::size_t offset = 0;

            for (std::size_t i = 0; i < 256; ++i) {
                IDT::SetEntry(
                    i,
                    Descriptors::GDT::g_codeSegmentOffset,
                    static_cast<void*>(__isr_stubs_start__ + offset),
                    commonFlags
                );

                IDT::EnableEntry(i);

                offset += __isr_get_alignment__(i);
            }
        }
    }
}