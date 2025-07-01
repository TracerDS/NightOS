#include <serial.hpp>

#ifdef __NOS_SERIAL_DEBUG__
#   include <klibc/cassert>
#endif

extern "C" std::uint8_t __kernel_serial_read_byte__(std::uint16_t port) noexcept;
extern "C" std::uint16_t __kernel_serial_read_word__(std::uint16_t port) noexcept;
extern "C" std::uint32_t __kernel_serial_read_dword__(std::uint16_t port) noexcept;

extern "C" void __kernel_serial_write_byte__(std::uint16_t port, std::uint8_t data) noexcept;
extern "C" void __kernel_serial_write_word__(std::uint16_t port, std::uint16_t data) noexcept;
extern "C" void __kernel_serial_write_dword__(std::uint16_t port, std::uint32_t data) noexcept;

namespace Serial {
    // Interrupt enable register
    namespace IER {
        constexpr std::uint8_t RECEIVED_DATA_AVAILABLE  = 1 << 0;
        constexpr std::uint8_t TH_REGISTER_EMPTY        = 1 << 1;
        constexpr std::uint8_t RECEIVER_LINE_STATUS     = 1 << 2;
        constexpr std::uint8_t MODEM_STATUS             = 1 << 3;
    }

    // Line Control Register
    namespace LCR {
        constexpr std::uint8_t CHAR_5_BITS      = 0b00;
        constexpr std::uint8_t CHAR_6_BITS      = 0b01;
        constexpr std::uint8_t CHAR_7_BITS      = 0b10;
        constexpr std::uint8_t CHAR_8_BITS      = 0b11;
        constexpr std::uint8_t ONE_STOP_BIT     = 0 << 2;
        constexpr std::uint8_t TWO_STOP_BIT     = 1 << 2;
        constexpr std::uint8_t PARITY_NONE      = 0b000 << 3;
        constexpr std::uint8_t PARITY_ODD       = 0b001 << 3;
        constexpr std::uint8_t PARITY_EVEN      = 0b010 << 3;
        constexpr std::uint8_t PARITY_MARK      = 0b101 << 3;
        constexpr std::uint8_t PARITY_SPACE     = 0b111 << 3;
        constexpr std::uint8_t DLAB_ENABLE      = 1 << 7;
    }

    // First In First Out Control Register
    namespace FIFOCR {
        constexpr std::uint8_t FIFO_ENABLE          = 1 << 0;
        constexpr std::uint8_t CLEAR_RECEIVE_FIFO   = 1 << 1;
        constexpr std::uint8_t CLEAR_TRANSMIT_FIFO  = 1 << 2;
        constexpr std::uint8_t CLEAR_ALL_FIFOS      = CLEAR_RECEIVE_FIFO | CLEAR_TRANSMIT_FIFO;
        constexpr std::uint8_t DMA_MODE_SELECT      = 1 << 3;
        constexpr std::uint8_t INT_TRIGGER_BYTE1    = 0b00 << 6;
        constexpr std::uint8_t INT_TRIGGER_BYTE4    = 0b01 << 6;
        constexpr std::uint8_t INT_TRIGGER_BYTE8    = 0b10 << 6;
        constexpr std::uint8_t INT_TRIGGER_BYTE14   = 0b11 << 6;
    }

    // Interrupt Identification Register
    namespace IIR {
        constexpr std::uint8_t INT_PENDING                  = 0 << 0;
        constexpr std::uint8_t INT_NOT_PENDING              = 1 << 0;
        constexpr std::uint8_t INT_MODEM                    = 0b00 << 1;
        constexpr std::uint8_t INT_TH_REGISTER_EMPTY        = 0b01 << 1;
        constexpr std::uint8_t INT_RECEIVED_DATA_AVAILABLE  = 0b10 << 1;
        constexpr std::uint8_t INT_RECEIVER_LINE            = 0b11 << 1;

        constexpr std::uint8_t NO_FIFO                      = 0b00 << 6;
        constexpr std::uint8_t FIFO_UNUSABLE                = 0b01 << 6;
        constexpr std::uint8_t FIFO_ENABLED                 = 0b10 << 6;
    }

    // Modem Control Register
    namespace MCR {
        constexpr std::uint8_t DTR_PIN          = 1 << 0; // Data Terminal Ready
        constexpr std::uint8_t RTS_PIN          = 1 << 1; // Request To Send
        constexpr std::uint8_t OUT1_PIN         = 1 << 2; // OUT1
        constexpr std::uint8_t OUT2_PIN         = 1 << 3; // OUT2
        constexpr std::uint8_t LOOPBACK_MODE    = 1 << 4; // Enable loopback mode

        constexpr std::uint8_t IRQ_ENABLE = OUT2_PIN;
    }

    // Line Status Register
    namespace LSR {
        constexpr std::uint8_t DATA_READY                = 1 << 0; // Data is available to read
        constexpr std::uint8_t OVERRUN_ERROR             = 1 << 1; // Overrun error occurred
        constexpr std::uint8_t PARITY_ERROR              = 1 << 2; // Parity error occurred
        constexpr std::uint8_t FRAMING_ERROR             = 1 << 3; // Framing error occurred
        constexpr std::uint8_t BREAK_INTERRUPT           = 1 << 4; // Break interrupt occurred
        constexpr std::uint8_t TH_REGISTER_EMPTY         = 1 << 5; // Transmitter Holding Register is empty
        constexpr std::uint8_t TRANSMITTER_EMPTY         = 1 << 6; // Transmitter is empty
        constexpr std::uint8_t ERROR_IN_RECEIVER_FIFO    = 1 << 7; // Error in the receiver FIFO
    }

    // Modem Status Register
    namespace MSR {
        constexpr std::uint8_t DELTA_CTS_PIN        = 1 << 0; // Change in CTS pin
        constexpr std::uint8_t DELTA_DSR_PIN        = 1 << 1; // Change in DSR pin
        constexpr std::uint8_t TRAILING_EDGE_RI_PIN = 1 << 2; // RI input changed from low to high state
        constexpr std::uint8_t DELTA_DCD_PIN        = 1 << 3; // Change in DCD pin
        constexpr std::uint8_t CTS_PIN              = 1 << 4; // Inverted CTS Signal
        constexpr std::uint8_t DSR_PIN              = 1 << 5; // Inverted DSR Signal
        constexpr std::uint8_t RI_PIN               = 1 << 6; // Inverted RI Signal
        constexpr std::uint8_t DCD_PIN              = 1 << 7; // Inverted DCD Signal
    }

    constexpr std::uint32_t DAUB_RATE = 115200; // Default baud rate

    bool Initialize(std::uint16_t port) noexcept {
        WriteByte(port + 1, 0x00);              // Disable interrupts
        WriteByte(port + 3, LCR::DLAB_ENABLE);
        WriteByte(port + 0, 0x03);              // 115200 / X = DAUB rate. Set to 38400 bps (X = 3)
        WriteByte(port + 1, 0x00);              // High byte of divisor
        WriteByte(port + 3, LCR::CHAR_8_BITS | LCR::PARITY_NONE | LCR::ONE_STOP_BIT);
        WriteByte(port + 2, FIFOCR::FIFO_ENABLE | FIFOCR::CLEAR_ALL_FIFOS | FIFOCR::INT_TRIGGER_BYTE14);
        WriteByte(port + 4, MCR::IRQ_ENABLE | MCR::DTR_PIN | MCR::RTS_PIN);

        // Test if serial is faulty
        WriteByte(port + 4, MCR::RTS_PIN | MCR::OUT1_PIN | MCR::OUT2_PIN | MCR::LOOPBACK_MODE);
        WriteByte(port + 0, 0xDD);
        if (ReadByte(port + 0) != 0xDD) {
            return false; // Serial port is faulty
        }
        WriteByte(port + 4, MCR::DTR_PIN | MCR::RTS_PIN | MCR::OUT1_PIN | MCR::OUT2_PIN);

        return true;
    }

    bool __serial_is_ready_to_read__(std::uint16_t port) noexcept {
        auto read = __kernel_serial_read_byte__(port + 5);
        return (read & LSR::DATA_READY) != 0;
    }
    bool __serial_is_ready_to_write__(std::uint16_t port) noexcept {
        auto read = __kernel_serial_read_byte__(port + 5);
        return (read & LSR::TH_REGISTER_EMPTY) != 0;
    }

    constexpr bool __serial_is_com__(std::uint16_t port) noexcept {
        switch(port) {
            case Ports::COM1:
            case Ports::COM2:
            case Ports::COM3:
            case Ports::COM4:
            case Ports::COM5:
            case Ports::COM6:
            case Ports::COM7:
            case Ports::COM8:
                return true;
            default:
                return false;
        }
    }
    
    void __serial_wait_until_ready_to_read__(std::uint16_t port) noexcept {
#ifdef __NOS_SERIAL_DEBUG__
        static std::uint32_t __gs_timeout__ = 0;
        static constexpr std::uint32_t __gs_max_timeout__ = 1000;
#endif
        // Wait until data is ready
        while (!__serial_is_ready_to_read__(port)) {
#ifdef __NOS_SERIAL_DEBUG__
            ++__gs_timeout__;
            assert(__gs_timeout__ < __gs_max_timeout__); // Ensure we don't loop indefinitely
#endif
        }
    }

    void __serial_wait_until_ready_to_write__(std::uint16_t port) noexcept {
#ifdef __NOS_SERIAL_DEBUG__
        static std::uint32_t __gs_timeout__ = 0;
        static constexpr std::uint32_t __gs_max_timeout__ = 1000;
#endif
        // Wait until data is ready
        while (!__serial_is_ready_to_write__(port)) {
#ifdef __NOS_SERIAL_DEBUG__
            ++__gs_timeout__;
            assert(__gs_timeout__ < __gs_max_timeout__); // Ensure we don't loop indefinitely
#endif
        }
    }

    std::uint8_t ReadByte(std::uint16_t port) noexcept {
        __serial_wait_until_ready_to_read__(port);
        return __kernel_serial_read_byte__(port);
    }
    std::uint16_t ReadWord(std::uint16_t port) noexcept {
        __serial_wait_until_ready_to_read__(port);

        if (!__serial_is_com__(port)) {
            return __kernel_serial_read_word__(port);
        }
        std::uint8_t low = __kernel_serial_read_byte__(port);
        std::uint8_t high = __kernel_serial_read_byte__(port);
        return high << 8 | low;
    }
    std::uint32_t ReadDword(std::uint16_t port) noexcept {
        __serial_wait_until_ready_to_read__(port);
        
        if (!__serial_is_com__(port)) {
            return __kernel_serial_read_dword__(port);
        }
        std::uint8_t low = __kernel_serial_read_word__(port);
        std::uint8_t high = __kernel_serial_read_word__(port);
        return high << 16 | low;
    }
    
    void WriteByte(std::uint16_t port, std::uint8_t data) noexcept {
        __serial_wait_until_ready_to_write__(port);
        __kernel_serial_write_byte__(port, data);
    }
    void WriteWord(std::uint16_t port, std::uint16_t data) noexcept {
        __serial_wait_until_ready_to_write__(port);

        if (!__serial_is_com__(port)) {
            __kernel_serial_write_word__(port, data);
            return;
        }
        __kernel_serial_write_byte__(port, data & 0xFF);
        __kernel_serial_write_byte__(port, (data & 0xFF00) >> 8);
    }
    void WriteDword(std::uint16_t port, std::uint32_t data) noexcept {
        __serial_wait_until_ready_to_write__(port);

        if (!__serial_is_com__(port)) {
            __kernel_serial_write_dword__(port, data);
            return;
        }

        __kernel_serial_write_word__(port, data & 0xFFFF);
        __kernel_serial_write_word__(port, (data & 0xFFFF0000) >> 16);
    }
}