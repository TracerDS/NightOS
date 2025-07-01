#pragma once

#include <cstdint>

namespace IRQ {
    namespace PIC {        
        namespace PIC_Ports {
            constexpr auto PIC1 = 0x20; // Master PIC
            constexpr auto PIC2 = 0xA0; // Slave PIC

            constexpr auto PIC1_COMMAND = PIC1;
            constexpr auto PIC1_DATA = PIC1 + 1;

            constexpr auto PIC2_COMMAND = PIC2;
            constexpr auto PIC2_DATA = PIC2 + 1;

            constexpr auto PIT_CHANNEL0 = 0x40; // PIT channel 0 data port
            constexpr auto PIT_CHANNEL1 = 0x41; // PIT channel 1 data port
            constexpr auto PIT_CHANNEL2 = 0x42; // PIT channel 2 data port
            constexpr auto PIT_COMMAND = 0x43; // PIT command port
        }

        namespace PIC_Flags {
            constexpr auto PIC_EOI = 0x20; // End of Interrupt command code

            constexpr auto ICW1 = 0x10;
            constexpr auto ICW4_PRESENT = 0x1;
            
            constexpr auto ICW3_MASTER = 1 << 2; // Cascade mode
            constexpr auto ICW3_SLAVE = 0x2;

            constexpr auto ICW4 = 0x01; // 8086 mode

            constexpr auto ICW5_MASK_ALL = 0xFF; // Mask all IRQs
            constexpr auto ICW5_MASK_NONE = 0x00; // Unmask all IRQs

            constexpr auto PIT_LOBYTE = 0b01;
            constexpr auto PIT_HIBYTE = 0b10;
            constexpr auto PIT_LOHIBYTE = 0b11;

            constexpr auto PIT_RATE_GENERATOR = 0b010;
            constexpr auto PIT_SQUARE_GENERATOR = 0b010;
        }
        void PIC_Init() noexcept;
    }
    void IRQ_Init() noexcept;
    void IRQ_SetClock(std::uint16_t frequency) noexcept;
}