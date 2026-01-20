#include <descriptors/irq.hpp>
#include <descriptors/idt.hpp>
#include <descriptors/isr.hpp>

#include <interfaces/streams/serial.hpp>
#include <io.hpp>

extern "C" void __kernel_serial_write_byte__(std::uint16_t port, std::uint8_t data);

namespace NOS::Interrupts {
    namespace PIC {
        void Init() noexcept {
            __kernel_serial_write_byte__(Ports::PIC1_COMMAND, Flags::ICW1 | Flags::ICW4_PRESENT); // Initialize PIC1
            __kernel_serial_write_byte__(Ports::PIC2_COMMAND, Flags::ICW1 | Flags::ICW4_PRESENT); // Initialize PIC1

            // Remap PIC
            __kernel_serial_write_byte__(Ports::PIC1_DATA, 0x20); // IRQ0-IRQ7 → ISR32-ISR39
            __kernel_serial_write_byte__(Ports::PIC2_DATA, 0x28); // IRQ8-IRQ15 → ISR40-ISR47

            // Setup cascading mode
            // Tell PIC1 that there is a slave PIC at IRQ2
            __kernel_serial_write_byte__(Ports::PIC1_DATA, Flags::ICW3_MASTER);
            // Tell PIC2 that it is a slave PIC
            __kernel_serial_write_byte__(Ports::PIC2_DATA, Flags::ICW3_SLAVE);
            
            // Set up env info (8086 mode)
            __kernel_serial_write_byte__(Ports::PIC1_DATA, Flags::ICW4);
            __kernel_serial_write_byte__(Ports::PIC2_DATA, Flags::ICW4);

            // Enable all IRQs
            __kernel_serial_write_byte__(Ports::PIC1_DATA, Flags::ICW5_MASK_NONE);
            __kernel_serial_write_byte__(Ports::PIC2_DATA, Flags::ICW5_MASK_NONE);
        }
    }

    namespace IRQ {
        void Init() noexcept {
            PIC::Init(); // Initialize the Programmable Interrupt Controller (PIC)
            
            SetClock(1000); // 1 ms
        }

        void SetClock(std::uint16_t frequency) noexcept { 
            // Default frequency for the PIT (Programmable Interval Timer)
            constexpr std::uint32_t IRQ_FREQUENCY = 1193182;

            // Set IRQ0 timer frequency
            std::uint32_t divisor = IRQ_FREQUENCY / frequency;
            if (divisor > 65535) divisor = 65535; // Limit divisor to 16 bits
            if (divisor == 0) divisor = 1;

            // Command port: channel 0, lo/hi byte, mode 3
            __kernel_serial_write_byte__(
                PIC::Ports::PIT_COMMAND,
                PIC::Flags::PIT_HIBYTE << 4 |
                PIC::Flags::PIT_RATE_GENERATOR << 1
            );
            
            // Low byte
            __kernel_serial_write_byte__(PIC::Ports::PIT_CHANNEL0, divisor & 0xFF);
            // High byte
            __kernel_serial_write_byte__(PIC::Ports::PIT_CHANNEL0, (divisor >> 8) & 0xFF);
        }
    }
    void Init() noexcept {
        ISR::Init();
        IDT::Init();
        IRQ::Init();
    }
}