#include <serial.hpp>
#include <io.hpp>

#ifdef __NOS_SERIAL_DEBUG__
#   include <klibc/cassert>
#endif

#include <utility>

namespace Serial {
    extern "C" std::uint8_t __kernel_serial_read_byte__(Serial::Port port) noexcept;
    extern "C" std::uint16_t __kernel_serial_read_word__(Serial::Port port) noexcept;
    extern "C" std::uint32_t __kernel_serial_read_dword__(Serial::Port port) noexcept;
    
    extern "C" void __kernel_serial_write_byte__(Serial::Port port, std::uint8_t data) noexcept;
    extern "C" void __kernel_serial_write_word__(Serial::Port port, std::uint16_t data) noexcept;
    extern "C" void __kernel_serial_write_dword__(Serial::Port port, std::uint32_t data) noexcept;


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

    constexpr bool __serial_is_com__(Serial::Port port) noexcept {
        switch(port) {
            case Serial::COM1:
            case Serial::COM2:
            case Serial::COM3:
            case Serial::COM4:
            case Serial::COM5:
            case Serial::COM6:
            case Serial::COM7:
            case Serial::COM8:
                return true;
            default:
                return false;
        }
    }
    bool __serial_is_ready_to_read__(Serial::Port port) noexcept {
        auto read = __kernel_serial_read_byte__(port + 5);
        return Utils::Bits::IsBitMaskSet(read, LSR::DATA_READY);
    }

    bool __serial_is_ready_to_write__(Serial::Port port) noexcept {
        auto read = __kernel_serial_read_byte__(port + 5);
        return Utils::Bits::IsBitMaskSet(read, LSR::TH_REGISTER_EMPTY);
    }
    
#ifdef __NOS_SERIAL_DEBUG__
    static constexpr std::uint64_t __gs_max_wait_timeout__ = 10'0000;
#endif

    void __serial_wait_until_ready_to_read__(Serial::Port port) noexcept {
#ifdef __NOS_SERIAL_DEBUG__
        static std::uint64_t __gs_timeout__ = 0;
#endif
        // Wait until data is ready
        while (!__serial_is_ready_to_read__(port)) {
#ifdef __NOS_SERIAL_DEBUG__
            ++__gs_timeout__;
            assert(__gs_timeout__ < __gs_max_wait_timeout__); // Ensure we don't loop indefinitely
#endif
        }
        __gs_timeout__ = 0;
    }

    void __serial_wait_until_ready_to_write__(Serial::Port port) noexcept {
#ifdef __NOS_SERIAL_DEBUG__
        static std::uint64_t __gs_timeout__ = 0;
#endif
        // Wait until data is ready
        while (!__serial_is_ready_to_write__(port)) {
#ifdef __NOS_SERIAL_DEBUG__
            ++__gs_timeout__;
            assert(__gs_timeout__ < __gs_max_wait_timeout__); // Ensure we don't loop indefinitely
#endif
        }
        __gs_timeout__ = 0;
    }

    Serial g_serial{};

    void Serial::init(Port port) noexcept {
        if (!__serial_is_com__(port)) {
            IO::kprintf("Serial::init: Unsupported port\r\n");
            return;
        }

        __kernel_serial_write_byte__(port + 1, 0x00);              // Disable interrupts
        __kernel_serial_write_byte__(port + 3, LCR::DLAB_ENABLE);  // Enable DLAB to set baud rate divisor
        
        // 115200 / X = DAUB rate.
        __kernel_serial_write_byte__(port + 0, 0x03);              // Set to 3 (lo byte) (38400 bps)
        __kernel_serial_write_byte__(port + 1, 0x00);              //          (hi byte)

        __kernel_serial_write_byte__(
            port + 3,
            LCR::CHAR_8_BITS | LCR::PARITY_NONE | LCR::ONE_STOP_BIT
        );
        __kernel_serial_write_byte__(
            port + 2,
            FIFOCR::FIFO_ENABLE | FIFOCR::CLEAR_ALL_FIFOS | FIFOCR::INT_TRIGGER_BYTE14
        );
        __kernel_serial_write_byte__(
            port + 4,
            MCR::IRQ_ENABLE | MCR::DTR_PIN | MCR::RTS_PIN
        );
    }

    void Serial::write_string(Port port, const char* const str) noexcept {
        for (std::size_t i = 0; str[i] != '\0'; ++i) {
            write_byte(port, static_cast<std::uint8_t>(str[i]));
        }
    }

    std::uint8_t Serial::read_byte(Port port) noexcept {
        __serial_wait_until_ready_to_read__(port);
        return __kernel_serial_read_byte__(port);
    }
    std::uint16_t Serial::read_word(Port port) noexcept {
        __serial_wait_until_ready_to_read__(port);

        if (!__serial_is_com__(port)) {
            return __kernel_serial_read_word__(port);
        }
        std::uint8_t low = __kernel_serial_read_byte__(port);
        std::uint8_t high = __kernel_serial_read_byte__(port);
        return high << 8 | low;
    }
    std::uint32_t Serial::read_dword(Port port) noexcept {
        __serial_wait_until_ready_to_read__(port);
        
        if (!__serial_is_com__(port)) {
            return __kernel_serial_read_dword__(port);
        }
        std::uint8_t low = __kernel_serial_read_word__(port);
        std::uint8_t high = __kernel_serial_read_word__(port);
        return high << 16 | low;
    }
    
    void Serial::write_byte(Port port, std::uint8_t data) noexcept {
        __serial_wait_until_ready_to_write__(port);
        __kernel_serial_write_byte__(port, data);
    }
    void Serial::write_word(Port port, std::uint16_t data) noexcept {
        __serial_wait_until_ready_to_write__(port);
        __kernel_serial_write_word__(port, data);
    }
    void Serial::write_dword(Port port, std::uint32_t data) noexcept {
        __serial_wait_until_ready_to_write__(port);
        __kernel_serial_write_dword__(port, data);
    }
}