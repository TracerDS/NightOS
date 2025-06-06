#include <descriptors/isr.hpp>
#include <descriptors/idt.hpp>
#include <descriptors/gdt.hpp>
#include <descriptors/descriptors.hpp>
#include <descriptors/irq.hpp>
#include <io.hpp>
#include <serial.hpp>

#include <utility>

namespace ISR {
    constexpr std::uint16_t ISR_COUNT = 256;

    namespace Handlers {
        extern "C" void ISR0();
        extern "C" void ISR1();
        extern "C" void ISR2();
        extern "C" void ISR3();
        extern "C" void ISR4();
        extern "C" void ISR5();
        extern "C" void ISR6();
        extern "C" void ISR7();
        extern "C" void ISR8();
        extern "C" void ISR9();
        extern "C" void ISR10();
        extern "C" void ISR11();
        extern "C" void ISR12();
        extern "C" void ISR13();
        extern "C" void ISR14();
        extern "C" void ISR15();
        extern "C" void ISR16();
        extern "C" void ISR17();
        extern "C" void ISR18();
        extern "C" void ISR19();
        extern "C" void ISR20();
        extern "C" void ISR21();
        extern "C" void ISR22();
        extern "C" void ISR23();
        extern "C" void ISR24();
        extern "C" void ISR25();
        extern "C" void ISR26();
        extern "C" void ISR27();
        extern "C" void ISR28();
        extern "C" void ISR29();
        extern "C" void ISR30();
        extern "C" void ISR31();
        extern "C" void ISR32();
        extern "C" void ISR33();
        extern "C" void ISR34();
        extern "C" void ISR35();
        extern "C" void ISR36();
        extern "C" void ISR37();
        extern "C" void ISR38();
        extern "C" void ISR39();
        extern "C" void ISR40();
        extern "C" void ISR41();
        extern "C" void ISR42();
        extern "C" void ISR43();
        extern "C" void ISR44();
        extern "C" void ISR45();
        extern "C" void ISR46();
        extern "C" void ISR47();
        extern "C" void ISR48();
        extern "C" void ISR49();
        extern "C" void ISR50();
        extern "C" void ISR51();
        extern "C" void ISR52();
        extern "C" void ISR53();
        extern "C" void ISR54();
        extern "C" void ISR55();
        extern "C" void ISR56();
        extern "C" void ISR57();
        extern "C" void ISR58();
        extern "C" void ISR59();
        extern "C" void ISR60();
        extern "C" void ISR61();
        extern "C" void ISR62();
        extern "C" void ISR63();
        extern "C" void ISR64();
        extern "C" void ISR65();
        extern "C" void ISR66();
        extern "C" void ISR67();
        extern "C" void ISR68();
        extern "C" void ISR69();
        extern "C" void ISR70();
        extern "C" void ISR71();
        extern "C" void ISR72();
        extern "C" void ISR73();
        extern "C" void ISR74();
        extern "C" void ISR75();
        extern "C" void ISR76();
        extern "C" void ISR77();
        extern "C" void ISR78();
        extern "C" void ISR79();
        extern "C" void ISR80();
        extern "C" void ISR81();
        extern "C" void ISR82();
        extern "C" void ISR83();
        extern "C" void ISR84();
        extern "C" void ISR85();
        extern "C" void ISR86();
        extern "C" void ISR87();
        extern "C" void ISR88();
        extern "C" void ISR89();
        extern "C" void ISR90();
        extern "C" void ISR91();
        extern "C" void ISR92();
        extern "C" void ISR93();
        extern "C" void ISR94();
        extern "C" void ISR95();
        extern "C" void ISR96();
        extern "C" void ISR97();
        extern "C" void ISR98();
        extern "C" void ISR99();
        extern "C" void ISR100();
        extern "C" void ISR101();
        extern "C" void ISR102();
        extern "C" void ISR103();
        extern "C" void ISR104();
        extern "C" void ISR105();
        extern "C" void ISR106();
        extern "C" void ISR107();
        extern "C" void ISR108();
        extern "C" void ISR109();
        extern "C" void ISR110();
        extern "C" void ISR111();
        extern "C" void ISR112();
        extern "C" void ISR113();
        extern "C" void ISR114();
        extern "C" void ISR115();
        extern "C" void ISR116();
        extern "C" void ISR117();
        extern "C" void ISR118();
        extern "C" void ISR119();
        extern "C" void ISR120();
        extern "C" void ISR121();
        extern "C" void ISR122();
        extern "C" void ISR123();
        extern "C" void ISR124();
        extern "C" void ISR125();
        extern "C" void ISR126();
        extern "C" void ISR127();
        extern "C" void ISR128();
        extern "C" void ISR129();
        extern "C" void ISR130();
        extern "C" void ISR131();
        extern "C" void ISR132();
        extern "C" void ISR133();
        extern "C" void ISR134();
        extern "C" void ISR135();
        extern "C" void ISR136();
        extern "C" void ISR137();
        extern "C" void ISR138();
        extern "C" void ISR139();
        extern "C" void ISR140();
        extern "C" void ISR141();
        extern "C" void ISR142();
        extern "C" void ISR143();
        extern "C" void ISR144();
        extern "C" void ISR145();
        extern "C" void ISR146();
        extern "C" void ISR147();
        extern "C" void ISR148();
        extern "C" void ISR149();
        extern "C" void ISR150();
        extern "C" void ISR151();
        extern "C" void ISR152();
        extern "C" void ISR153();
        extern "C" void ISR154();
        extern "C" void ISR155();
        extern "C" void ISR156();
        extern "C" void ISR157();
        extern "C" void ISR158();
        extern "C" void ISR159();
        extern "C" void ISR160();
        extern "C" void ISR161();
        extern "C" void ISR162();
        extern "C" void ISR163();
        extern "C" void ISR164();
        extern "C" void ISR165();
        extern "C" void ISR166();
        extern "C" void ISR167();
        extern "C" void ISR168();
        extern "C" void ISR169();
        extern "C" void ISR170();
        extern "C" void ISR171();
        extern "C" void ISR172();
        extern "C" void ISR173();
        extern "C" void ISR174();
        extern "C" void ISR175();
        extern "C" void ISR176();
        extern "C" void ISR177();
        extern "C" void ISR178();
        extern "C" void ISR179();
        extern "C" void ISR180();
        extern "C" void ISR181();
        extern "C" void ISR182();
        extern "C" void ISR183();
        extern "C" void ISR184();
        extern "C" void ISR185();
        extern "C" void ISR186();
        extern "C" void ISR187();
        extern "C" void ISR188();
        extern "C" void ISR189();
        extern "C" void ISR190();
        extern "C" void ISR191();
        extern "C" void ISR192();
        extern "C" void ISR193();
        extern "C" void ISR194();
        extern "C" void ISR195();
        extern "C" void ISR196();
        extern "C" void ISR197();
        extern "C" void ISR198();
        extern "C" void ISR199();
        extern "C" void ISR200();
        extern "C" void ISR201();
        extern "C" void ISR202();
        extern "C" void ISR203();
        extern "C" void ISR204();
        extern "C" void ISR205();
        extern "C" void ISR206();
        extern "C" void ISR207();
        extern "C" void ISR208();
        extern "C" void ISR209();
        extern "C" void ISR210();
        extern "C" void ISR211();
        extern "C" void ISR212();
        extern "C" void ISR213();
        extern "C" void ISR214();
        extern "C" void ISR215();
        extern "C" void ISR216();
        extern "C" void ISR217();
        extern "C" void ISR218();
        extern "C" void ISR219();
        extern "C" void ISR220();
        extern "C" void ISR221();
        extern "C" void ISR222();
        extern "C" void ISR223();
        extern "C" void ISR224();
        extern "C" void ISR225();
        extern "C" void ISR226();
        extern "C" void ISR227();
        extern "C" void ISR228();
        extern "C" void ISR229();
        extern "C" void ISR230();
        extern "C" void ISR231();
        extern "C" void ISR232();
        extern "C" void ISR233();
        extern "C" void ISR234();
        extern "C" void ISR235();
        extern "C" void ISR236();
        extern "C" void ISR237();
        extern "C" void ISR238();
        extern "C" void ISR239();
        extern "C" void ISR240();
        extern "C" void ISR241();
        extern "C" void ISR242();
        extern "C" void ISR243();
        extern "C" void ISR244();
        extern "C" void ISR245();
        extern "C" void ISR246();
        extern "C" void ISR247();
        extern "C" void ISR248();
        extern "C" void ISR249();
        extern "C" void ISR250();
        extern "C" void ISR251();
        extern "C" void ISR252();
        extern "C" void ISR253();
        extern "C" void ISR254();
        extern "C" void ISR255();
    }

    ISRHandler g_ISRHandlers[ISR_COUNT] {};
    constexpr const char* g_ISRExceptions[] = {
        "Divide by zero error",
        "Debug exception",
        "Non-maskable Interrupt",
        "Breakpoint",
        "Overflow",
        "Bound Range Exceeded",
        "Invalid Opcode",
        "Device Not Available",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Invalid TSS",
        "Segment Not Present",
        "Stack-Segment Fault",
        "General Protection Fault",
        "Page Fault",
        "",
        "x87 Floating-Point Exception",
        "Alignment Check",
        "Machine Check",
        "SIMD Floating-Point Exception",
        "Virtualization Exception",
        "Control Protection Exception ",
    };

    void ISR_RegisterHandler(std::uint8_t interrupt, ISRHandler handler) noexcept {
        g_ISRHandlers[interrupt] = handler;
    }

    void ISR_UnregisterHandler(std::uint8_t interrupt) noexcept {
        g_ISRHandlers[interrupt] = nullptr;
    }

    void ISR_DefaultHandler(ISR_RegistersState* regs) {
        const char* errMsg = sizeof(g_ISRExceptions) / sizeof(g_ISRExceptions[0]) > regs->interrupt
            ? g_ISRExceptions[regs->interrupt]
            : "Unknown Exception";

        IO::kprintf_color(
            "\r\nISR Handler %d called!\r\n"
            "Exception: %s\r\n"
            "Code: 0x%X\r\n"
            "Registers:\r\n"
            "  EAX    = 0x%X\r\n"
            "  EBX    = 0x%X\r\n"
            "  ECX    = 0x%X\r\n"
            "  EDX    = 0x%X\r\n"
            "  ESI    = 0x%X\r\n"
            "  EDI    = 0x%X\r\n"
            "  EBP    = 0x%X\r\n"
            "  ESP    = 0x%X\r\n"
            "  CS     = 0x%X\r\n"
            "  DS     = 0x%X\r\n"
            "  ES     = 0x%X\r\n"
            "  FS     = 0x%X\r\n"
            "  GS     = 0x%X\r\n"
            "  SS     = 0x%X\r\n"
            "  EIP    = 0x%X\r\n"
            "  EFLAGS = 0x%X\r\n",
            Terminal::Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
            Terminal::Terminal::VGAColor::VGA_COLOR_BLACK,
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

    extern "C" void ISR_Handler(ISR_RegistersState* regs) {
        if (g_ISRHandlers[regs->interrupt] != nullptr) {
            g_ISRHandlers[regs->interrupt](regs);
            return;
        }

        ISR_DefaultHandler(regs);
        
        if (regs->interrupt == 0x0E) {
            __asm__ volatile("hlt");
            return;
        }

        if (regs->interrupt >= 0x20) {
            if (regs->interrupt >= 0x28) {
                Serial::WriteByte(IRQ::PIC::PIC_Ports::PIC2, IRQ::PIC::PIC_Flags::PIC_EOI); // Send EOI to slave PIC
            }
            Serial::WriteByte(IRQ::PIC::PIC_Ports::PIC1, IRQ::PIC::PIC_Flags::PIC_EOI); // Send EOI to master PIC
            return;
        }
    }
    
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

    extern "C" std::uint8_t __isr_stubs_start__[];
    extern "C" std::uint8_t __isr_stubs_end__[];

    void ISR_Initialize() {
        constexpr auto commonFlags =
            std::to_underlying<>(IDT::IDT_GateType::GATE_INT32) |
            std::to_underlying<>(Descriptors::PrivilegeType::PT_KERNEL);

        std::size_t offset = 0;
        /*
        for (std::size_t i = 0; i < 256; ++i) {
            IDT::IDT_SetEntry (
                i,
                GDT::g_CodeSegmentOffset,
                static_cast<void*>(__isr_stubs_start__ + offset),
                std::to_underlying<>(IDT::IDT_GateType::GATE_INT32) |
                std::to_underlying<>(Descriptors::PrivilegeType::PT_KERNEL)
            );

            IDT::IDT_EnableEntry(i);

            offset += __isr_get_alignment__(i);
        }
        */
        {
            ISR_RegisterHandler(0, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(1, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(2, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(3, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(4, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(5, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(6, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(7, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(8, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(9, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(10, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(11, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(12, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(13, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(14, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(15, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(16, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(17, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(18, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(19, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(20, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(21, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(22, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(23, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(24, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(25, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(26, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(27, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(28, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(29, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(30, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(31, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(32, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(33, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(34, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(35, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(36, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(37, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(38, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(39, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(40, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(41, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(42, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(43, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(44, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(45, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(46, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(47, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(48, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(49, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(50, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(51, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(52, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(53, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(54, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(55, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(56, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(57, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(58, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(59, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(60, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(61, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(62, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(63, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(64, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(65, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(66, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(67, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(68, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(69, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(70, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(71, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(72, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(73, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(74, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(75, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(76, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(77, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(78, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(79, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(80, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(81, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(82, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(83, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(84, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(85, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(86, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(87, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(88, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(89, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(90, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(91, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(92, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(93, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(94, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(95, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(96, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(97, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(98, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(99, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(100, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(101, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(102, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(103, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(104, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(105, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(106, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(107, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(108, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(109, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(110, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(111, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(112, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(113, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(114, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(115, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(116, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(117, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(118, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(119, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(120, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(121, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(122, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(123, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(124, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(125, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(126, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(127, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(128, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(129, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(130, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(131, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(132, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(133, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(134, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(135, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(136, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(137, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(138, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(139, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(140, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(141, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(142, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(143, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(144, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(145, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(146, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(147, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(148, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(149, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(150, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(151, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(152, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(153, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(154, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(155, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(156, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(157, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(158, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(159, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(160, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(161, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(162, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(163, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(164, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(165, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(166, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(167, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(168, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(169, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(170, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(171, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(172, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(173, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(174, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(175, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(176, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(177, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(178, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(179, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(180, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(181, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(182, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(183, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(184, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(185, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(186, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(187, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(188, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(189, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(190, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(191, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(192, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(193, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(194, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(195, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(196, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(197, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(198, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(199, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(200, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(201, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(202, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(203, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(204, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(205, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(206, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(207, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(208, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(209, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(210, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(211, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(212, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(213, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(214, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(215, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(216, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(217, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(218, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(219, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(220, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(221, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(222, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(223, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(224, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(225, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(226, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(227, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(228, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(229, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(230, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(231, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(232, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(233, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(234, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(235, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(236, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(237, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(238, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(239, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(240, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(241, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(242, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(243, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(244, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(245, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(246, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(247, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(248, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(249, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(250, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(251, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(252, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(253, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(254, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            ISR_RegisterHandler(255, [](ISR_RegistersState* regs){ IO::kprintf("Interrupt %d called!\r\n", regs->interrupt); } );
            IDT::IDT_SetEntry(0,    GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR0),   commonFlags);
            IDT::IDT_SetEntry(1,    GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR1),   commonFlags);
            IDT::IDT_SetEntry(2,    GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR2),   commonFlags);
            IDT::IDT_SetEntry(3,    GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR3),   commonFlags);
            IDT::IDT_SetEntry(4,    GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR4),   commonFlags);
            IDT::IDT_SetEntry(5,    GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR5),   commonFlags);
            IDT::IDT_SetEntry(6,    GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR6),   commonFlags);
            IDT::IDT_SetEntry(7,    GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR7),   commonFlags);
            IDT::IDT_SetEntry(8,    GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR8),   commonFlags);
            IDT::IDT_SetEntry(9,    GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR9),   commonFlags);
            IDT::IDT_SetEntry(10,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR10),  commonFlags);
            IDT::IDT_SetEntry(11,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR11),  commonFlags);
            IDT::IDT_SetEntry(12,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR12),  commonFlags);
            IDT::IDT_SetEntry(13,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR13),  commonFlags);
            IDT::IDT_SetEntry(14,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR14),  commonFlags);
            IDT::IDT_SetEntry(15,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR15),  commonFlags);
            IDT::IDT_SetEntry(16,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR16),  commonFlags);
            IDT::IDT_SetEntry(17,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR17),  commonFlags);
            IDT::IDT_SetEntry(18,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR18),  commonFlags);
            IDT::IDT_SetEntry(19,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR19),  commonFlags);
            IDT::IDT_SetEntry(20,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR20),  commonFlags);
            IDT::IDT_SetEntry(21,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR21),  commonFlags);
            IDT::IDT_SetEntry(22,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR22),  commonFlags);
            IDT::IDT_SetEntry(23,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR23),  commonFlags);
            IDT::IDT_SetEntry(24,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR24),  commonFlags);
            IDT::IDT_SetEntry(25,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR25),  commonFlags);
            IDT::IDT_SetEntry(26,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR26),  commonFlags);
            IDT::IDT_SetEntry(27,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR27),  commonFlags);
            IDT::IDT_SetEntry(28,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR28),  commonFlags);
            IDT::IDT_SetEntry(29,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR29),  commonFlags);
            IDT::IDT_SetEntry(30,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR30),  commonFlags);
            IDT::IDT_SetEntry(31,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR31),  commonFlags);
            IDT::IDT_SetEntry(32,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR32),  commonFlags);
            IDT::IDT_SetEntry(33,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR33),  commonFlags);
            IDT::IDT_SetEntry(34,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR34),  commonFlags);
            IDT::IDT_SetEntry(35,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR35),  commonFlags);
            IDT::IDT_SetEntry(36,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR36),  commonFlags);
            IDT::IDT_SetEntry(37,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR37),  commonFlags);
            IDT::IDT_SetEntry(38,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR38),  commonFlags);
            IDT::IDT_SetEntry(39,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR39),  commonFlags);
            IDT::IDT_SetEntry(40,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR40),  commonFlags);
            IDT::IDT_SetEntry(41,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR41),  commonFlags);
            IDT::IDT_SetEntry(42,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR42),  commonFlags);
            IDT::IDT_SetEntry(43,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR43),  commonFlags);
            IDT::IDT_SetEntry(44,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR44),  commonFlags);
            IDT::IDT_SetEntry(45,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR45),  commonFlags);
            IDT::IDT_SetEntry(46,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR46),  commonFlags);
            IDT::IDT_SetEntry(47,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR47),  commonFlags);
            IDT::IDT_SetEntry(48,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR48),  commonFlags);
            IDT::IDT_SetEntry(49,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR49),  commonFlags);
            IDT::IDT_SetEntry(50,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR50),  commonFlags);
            IDT::IDT_SetEntry(51,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR51),  commonFlags);
            IDT::IDT_SetEntry(52,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR52),  commonFlags);
            IDT::IDT_SetEntry(53,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR53),  commonFlags);
            IDT::IDT_SetEntry(54,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR54),  commonFlags);
            IDT::IDT_SetEntry(55,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR55),  commonFlags);
            IDT::IDT_SetEntry(56,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR56),  commonFlags);
            IDT::IDT_SetEntry(57,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR57),  commonFlags);
            IDT::IDT_SetEntry(58,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR58),  commonFlags);
            IDT::IDT_SetEntry(59,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR59),  commonFlags);
            IDT::IDT_SetEntry(60,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR60),  commonFlags);
            IDT::IDT_SetEntry(61,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR61),  commonFlags);
            IDT::IDT_SetEntry(62,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR62),  commonFlags);
            IDT::IDT_SetEntry(63,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR63),  commonFlags);
            IDT::IDT_SetEntry(64,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR64),  commonFlags);
            IDT::IDT_SetEntry(65,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR65),  commonFlags);
            IDT::IDT_SetEntry(66,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR66),  commonFlags);
            IDT::IDT_SetEntry(67,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR67),  commonFlags);
            IDT::IDT_SetEntry(68,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR68),  commonFlags);
            IDT::IDT_SetEntry(69,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR69),  commonFlags);
            IDT::IDT_SetEntry(70,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR70),  commonFlags);
            IDT::IDT_SetEntry(71,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR71),  commonFlags);
            IDT::IDT_SetEntry(72,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR72),  commonFlags);
            IDT::IDT_SetEntry(73,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR73),  commonFlags);
            IDT::IDT_SetEntry(74,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR74),  commonFlags);
            IDT::IDT_SetEntry(75,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR75),  commonFlags);
            IDT::IDT_SetEntry(76,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR76),  commonFlags);
            IDT::IDT_SetEntry(77,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR77),  commonFlags);
            IDT::IDT_SetEntry(78,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR78),  commonFlags);
            IDT::IDT_SetEntry(79,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR79),  commonFlags);
            IDT::IDT_SetEntry(80,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR80),  commonFlags);
            IDT::IDT_SetEntry(81,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR81),  commonFlags);
            IDT::IDT_SetEntry(82,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR82),  commonFlags);
            IDT::IDT_SetEntry(83,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR83),  commonFlags);
            IDT::IDT_SetEntry(84,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR84),  commonFlags);
            IDT::IDT_SetEntry(85,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR85),  commonFlags);
            IDT::IDT_SetEntry(86,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR86),  commonFlags);
            IDT::IDT_SetEntry(87,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR87),  commonFlags);
            IDT::IDT_SetEntry(88,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR88),  commonFlags);
            IDT::IDT_SetEntry(89,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR89),  commonFlags);
            IDT::IDT_SetEntry(90,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR90),  commonFlags);
            IDT::IDT_SetEntry(91,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR91),  commonFlags);
            IDT::IDT_SetEntry(92,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR92),  commonFlags);
            IDT::IDT_SetEntry(93,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR93),  commonFlags);
            IDT::IDT_SetEntry(94,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR94),  commonFlags);
            IDT::IDT_SetEntry(95,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR95),  commonFlags);
            IDT::IDT_SetEntry(96,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR96),  commonFlags);
            IDT::IDT_SetEntry(97,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR97),  commonFlags);
            IDT::IDT_SetEntry(98,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR98),  commonFlags);
            IDT::IDT_SetEntry(99,   GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR99),  commonFlags);
            IDT::IDT_SetEntry(100,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR100), commonFlags);
            IDT::IDT_SetEntry(101,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR101), commonFlags);
            IDT::IDT_SetEntry(102,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR102), commonFlags);
            IDT::IDT_SetEntry(103,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR103), commonFlags);
            IDT::IDT_SetEntry(104,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR104), commonFlags);
            IDT::IDT_SetEntry(105,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR105), commonFlags);
            IDT::IDT_SetEntry(106,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR106), commonFlags);
            IDT::IDT_SetEntry(107,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR107), commonFlags);
            IDT::IDT_SetEntry(108,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR108), commonFlags);
            IDT::IDT_SetEntry(109,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR109), commonFlags);
            IDT::IDT_SetEntry(110,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR110), commonFlags);
            IDT::IDT_SetEntry(111,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR111), commonFlags);
            IDT::IDT_SetEntry(112,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR112), commonFlags);
            IDT::IDT_SetEntry(113,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR113), commonFlags);
            IDT::IDT_SetEntry(114,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR114), commonFlags);
            IDT::IDT_SetEntry(115,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR115), commonFlags);
            IDT::IDT_SetEntry(116,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR116), commonFlags);
            IDT::IDT_SetEntry(117,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR117), commonFlags);
            IDT::IDT_SetEntry(118,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR118), commonFlags);
            IDT::IDT_SetEntry(119,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR119), commonFlags);
            IDT::IDT_SetEntry(120,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR120), commonFlags);
            IDT::IDT_SetEntry(121,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR121), commonFlags);
            IDT::IDT_SetEntry(122,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR122), commonFlags);
            IDT::IDT_SetEntry(123,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR123), commonFlags);
            IDT::IDT_SetEntry(124,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR124), commonFlags);
            IDT::IDT_SetEntry(125,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR125), commonFlags);
            IDT::IDT_SetEntry(126,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR126), commonFlags);
            IDT::IDT_SetEntry(127,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR127), commonFlags);
            IDT::IDT_SetEntry(128,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR128), commonFlags);
            IDT::IDT_SetEntry(129,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR129), commonFlags);
            IDT::IDT_SetEntry(130,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR130), commonFlags);
            IDT::IDT_SetEntry(131,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR131), commonFlags);
            IDT::IDT_SetEntry(132,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR132), commonFlags);
            IDT::IDT_SetEntry(133,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR133), commonFlags);
            IDT::IDT_SetEntry(134,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR134), commonFlags);
            IDT::IDT_SetEntry(135,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR135), commonFlags);
            IDT::IDT_SetEntry(136,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR136), commonFlags);
            IDT::IDT_SetEntry(137,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR137), commonFlags);
            IDT::IDT_SetEntry(138,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR138), commonFlags);
            IDT::IDT_SetEntry(139,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR139), commonFlags);
            IDT::IDT_SetEntry(140,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR140), commonFlags);
            IDT::IDT_SetEntry(141,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR141), commonFlags);
            IDT::IDT_SetEntry(142,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR142), commonFlags);
            IDT::IDT_SetEntry(143,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR143), commonFlags);
            IDT::IDT_SetEntry(144,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR144), commonFlags);
            IDT::IDT_SetEntry(145,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR145), commonFlags);
            IDT::IDT_SetEntry(146,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR146), commonFlags);
            IDT::IDT_SetEntry(147,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR147), commonFlags);
            IDT::IDT_SetEntry(148,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR148), commonFlags);
            IDT::IDT_SetEntry(149,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR149), commonFlags);
            IDT::IDT_SetEntry(150,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR150), commonFlags);
            IDT::IDT_SetEntry(151,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR151), commonFlags);
            IDT::IDT_SetEntry(152,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR152), commonFlags);
            IDT::IDT_SetEntry(153,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR153), commonFlags);
            IDT::IDT_SetEntry(154,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR154), commonFlags);
            IDT::IDT_SetEntry(155,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR155), commonFlags);
            IDT::IDT_SetEntry(156,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR156), commonFlags);
            IDT::IDT_SetEntry(157,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR157), commonFlags);
            IDT::IDT_SetEntry(158,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR158), commonFlags);
            IDT::IDT_SetEntry(159,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR159), commonFlags);
            IDT::IDT_SetEntry(160,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR160), commonFlags);
            IDT::IDT_SetEntry(161,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR161), commonFlags);
            IDT::IDT_SetEntry(162,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR162), commonFlags);
            IDT::IDT_SetEntry(163,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR163), commonFlags);
            IDT::IDT_SetEntry(164,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR164), commonFlags);
            IDT::IDT_SetEntry(165,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR165), commonFlags);
            IDT::IDT_SetEntry(166,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR166), commonFlags);
            IDT::IDT_SetEntry(167,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR167), commonFlags);
            IDT::IDT_SetEntry(168,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR168), commonFlags);
            IDT::IDT_SetEntry(169,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR169), commonFlags);
            IDT::IDT_SetEntry(170,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR170), commonFlags);
            IDT::IDT_SetEntry(171,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR171), commonFlags);
            IDT::IDT_SetEntry(172,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR172), commonFlags);
            IDT::IDT_SetEntry(173,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR173), commonFlags);
            IDT::IDT_SetEntry(174,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR174), commonFlags);
            IDT::IDT_SetEntry(175,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR175), commonFlags);
            IDT::IDT_SetEntry(176,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR176), commonFlags);
            IDT::IDT_SetEntry(177,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR177), commonFlags);
            IDT::IDT_SetEntry(178,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR178), commonFlags);
            IDT::IDT_SetEntry(179,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR179), commonFlags);
            IDT::IDT_SetEntry(180,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR180), commonFlags);
            IDT::IDT_SetEntry(181,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR181), commonFlags);
            IDT::IDT_SetEntry(182,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR182), commonFlags);
            IDT::IDT_SetEntry(183,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR183), commonFlags);
            IDT::IDT_SetEntry(184,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR184), commonFlags);
            IDT::IDT_SetEntry(185,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR185), commonFlags);
            IDT::IDT_SetEntry(186,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR186), commonFlags);
            IDT::IDT_SetEntry(187,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR187), commonFlags);
            IDT::IDT_SetEntry(188,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR188), commonFlags);
            IDT::IDT_SetEntry(189,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR189), commonFlags);
            IDT::IDT_SetEntry(190,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR190), commonFlags);
            IDT::IDT_SetEntry(191,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR191), commonFlags);
            IDT::IDT_SetEntry(192,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR192), commonFlags);
            IDT::IDT_SetEntry(193,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR193), commonFlags);
            IDT::IDT_SetEntry(194,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR194), commonFlags);
            IDT::IDT_SetEntry(195,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR195), commonFlags);
            IDT::IDT_SetEntry(196,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR196), commonFlags);
            IDT::IDT_SetEntry(197,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR197), commonFlags);
            IDT::IDT_SetEntry(198,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR198), commonFlags);
            IDT::IDT_SetEntry(199,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR199), commonFlags);            
            IDT::IDT_SetEntry(200,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR200), commonFlags);
            IDT::IDT_SetEntry(201,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR201), commonFlags);
            IDT::IDT_SetEntry(202,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR202), commonFlags);
            IDT::IDT_SetEntry(203,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR203), commonFlags);
            IDT::IDT_SetEntry(204,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR204), commonFlags);
            IDT::IDT_SetEntry(205,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR205), commonFlags);
            IDT::IDT_SetEntry(206,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR206), commonFlags);
            IDT::IDT_SetEntry(207,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR207), commonFlags);
            IDT::IDT_SetEntry(208,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR208), commonFlags);
            IDT::IDT_SetEntry(209,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR209), commonFlags);
            IDT::IDT_SetEntry(210,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR210), commonFlags);
            IDT::IDT_SetEntry(211,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR211), commonFlags);
            IDT::IDT_SetEntry(212,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR212), commonFlags);
            IDT::IDT_SetEntry(213,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR213), commonFlags);
            IDT::IDT_SetEntry(214,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR214), commonFlags);
            IDT::IDT_SetEntry(215,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR215), commonFlags);
            IDT::IDT_SetEntry(216,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR216), commonFlags);
            IDT::IDT_SetEntry(217,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR217), commonFlags);
            IDT::IDT_SetEntry(218,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR218), commonFlags);
            IDT::IDT_SetEntry(219,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR219), commonFlags);
            IDT::IDT_SetEntry(220,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR220), commonFlags);
            IDT::IDT_SetEntry(221,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR221), commonFlags);
            IDT::IDT_SetEntry(222,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR222), commonFlags);
            IDT::IDT_SetEntry(223,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR223), commonFlags);
            IDT::IDT_SetEntry(224,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR224), commonFlags);
            IDT::IDT_SetEntry(225,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR225), commonFlags);
            IDT::IDT_SetEntry(226,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR226), commonFlags);
            IDT::IDT_SetEntry(227,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR227), commonFlags);
            IDT::IDT_SetEntry(228,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR228), commonFlags);
            IDT::IDT_SetEntry(229,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR229), commonFlags);
            IDT::IDT_SetEntry(230,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR230), commonFlags);
            IDT::IDT_SetEntry(231,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR231), commonFlags);
            IDT::IDT_SetEntry(232,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR232), commonFlags);
            IDT::IDT_SetEntry(233,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR233), commonFlags);
            IDT::IDT_SetEntry(234,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR234), commonFlags);
            IDT::IDT_SetEntry(235,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR235), commonFlags);
            IDT::IDT_SetEntry(236,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR236), commonFlags);
            IDT::IDT_SetEntry(237,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR237), commonFlags);
            IDT::IDT_SetEntry(238,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR238), commonFlags);
            IDT::IDT_SetEntry(239,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR239), commonFlags);
            IDT::IDT_SetEntry(240,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR240), commonFlags);
            IDT::IDT_SetEntry(241,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR241), commonFlags);
            IDT::IDT_SetEntry(242,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR242), commonFlags);
            IDT::IDT_SetEntry(243,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR243), commonFlags);
            IDT::IDT_SetEntry(244,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR244), commonFlags);
            IDT::IDT_SetEntry(245,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR245), commonFlags);
            IDT::IDT_SetEntry(246,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR246), commonFlags);
            IDT::IDT_SetEntry(247,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR247), commonFlags);
            IDT::IDT_SetEntry(248,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR248), commonFlags);
            IDT::IDT_SetEntry(249,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR249), commonFlags);
            IDT::IDT_SetEntry(250,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR250), commonFlags);
            IDT::IDT_SetEntry(251,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR251), commonFlags);
            IDT::IDT_SetEntry(252,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR252), commonFlags);
            IDT::IDT_SetEntry(253,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR253), commonFlags);
            IDT::IDT_SetEntry(254,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR254), commonFlags);
            IDT::IDT_SetEntry(255,  GDT::g_CodeSegmentOffset, reinterpret_cast<void*>(Handlers::ISR255), commonFlags);

            for(auto i = 0; i < 256; ++i) {
                IDT::IDT_EnableEntry(i);
            }
        }

        if (false) {
            ISR_RegisterHandler(0x21, [](ISR_RegistersState* regs) {
                Serial::ReadByte(0x60); // Acknowledge the keyboard interrupt
            });

            ISR_RegisterHandler(0x20, [](ISR_RegistersState* regs) {
                const int timer = 10;
                static std::uint32_t ticks = 0;
                ticks++;
                if (ticks >= timer) {
                    ticks = 0;
                    IO::kprintf("Tick!\r\n");
                }
            });
        }
    }
}