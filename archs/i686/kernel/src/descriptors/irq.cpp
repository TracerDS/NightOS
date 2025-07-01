#include <descriptors/irq.hpp>
#include <serial.hpp>
#include <io.hpp>

namespace IRQ {    
    namespace PIC {
        void PIC_Init() noexcept {
            Serial::WriteByte(PIC_Ports::PIC1_COMMAND, PIC_Flags::ICW1 | PIC_Flags::ICW4_PRESENT); // Initialize PIC1
            Serial::WriteByte(PIC_Ports::PIC2_COMMAND, PIC_Flags::ICW1 | PIC_Flags::ICW4_PRESENT); // Initialize PIC1

            // Remap PIC
            Serial::WriteByte(PIC_Ports::PIC1_DATA, 0x20); // IRQ0-IRQ7 → ISR32-ISR39
            Serial::WriteByte(PIC_Ports::PIC2_DATA, 0x28); // IRQ8-IRQ15 → ISR40-ISR47

            // Setup cascading mode
            Serial::WriteByte(PIC_Ports::PIC1_DATA, PIC_Flags::ICW3_MASTER); // Tell PIC1 that there is a slave PIC at IRQ2
            Serial::WriteByte(PIC_Ports::PIC2_DATA, PIC_Flags::ICW3_SLAVE); // Tell PIC2 that it is a slave PIC
            
            // Set up env info (8086 mode)
            Serial::WriteByte(PIC_Ports::PIC1_DATA, PIC_Flags::ICW4);
            Serial::WriteByte(PIC_Ports::PIC2_DATA, PIC_Flags::ICW4);

            // Enable all IRQs
            Serial::WriteByte(PIC_Ports::PIC1_DATA, PIC_Flags::ICW5_MASK_NONE);
            Serial::WriteByte(PIC_Ports::PIC2_DATA, PIC_Flags::ICW5_MASK_NONE);
        }
    }


    void IRQ_Init() noexcept {
        PIC::PIC_Init(); // Initialize the Programmable Interrupt Controller (PIC)
        
        IRQ_SetClock(1000); // 1 ms
    }

    void IRQ_SetClock(std::uint16_t frequency) noexcept { 
        constexpr std::uint32_t IRQ_FREQUENCY = 1193182; // Default frequency for the PIT (Programmable Interval Timer)

        // Set IRQ0 timer frequency
        std::uint32_t divisor = IRQ_FREQUENCY / frequency;
        if (divisor > 65535) divisor = 65535; // Limit divisor to 16 bits
        if (divisor == 0) divisor = 1;

        // Command port: channel 0, lo/hi byte, mode 3
        Serial::WriteByte(
            PIC::PIC_Ports::PIT_COMMAND,
            PIC::PIC_Flags::PIT_HIBYTE << 4 |
            PIC::PIC_Flags::PIT_RATE_GENERATOR << 1
        );
        Serial::WriteByte(PIC::PIC_Ports::PIT_CHANNEL0, divisor & 0xFF);        // Low byte
        Serial::WriteByte(PIC::PIC_Ports::PIT_CHANNEL0, (divisor >> 8) & 0xFF); // High byte
    }
}