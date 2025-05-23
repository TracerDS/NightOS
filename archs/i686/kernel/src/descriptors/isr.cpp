#include <descriptors/isr.hpp>
#include <descriptors/idt.hpp>
#include <descriptors/gdt.hpp>
#include <descriptors/descriptors.hpp>
#include <terminal.hpp>

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

    void ISR_Initialize() {
        for(auto i = 0; i < ISR_COUNT; ++i) {
            IDT::IDT_SetEntry(
                i,
                GDT::g_CodeSegmentOffset,
                0,
                std::to_underlying<>(IDT::IDT_GateType::GATE_INT32) |
                std::to_underlying<>(Descriptors::PrivilegeType::PT_KERNEL) |
                std::to_underlying<>(GDT::AccessType::AT_INVALID_SEGMENT)
            );
        }
        ISR_RegisterHandler(0, Handlers::ISR0);
    }

    void ISR_RegisterHandler(std::uint8_t interrupt, ISRHandler handler) {
        g_ISRHandlers[interrupt] = handler;
        IDT::IDT_SetEntry(interrupt,
            GDT::g_CodeSegmentOffset,
            (void*)handler,
            std::to_underlying<>(IDT::IDT_GateType::GATE_INT32) |
            std::to_underlying<>(Descriptors::PrivilegeType::PT_KERNEL)
        );
        IDT::IDT_EnableEntry(interrupt);
    }

    extern "C" void ISR_Handler(ISR_RegistersState regs) {
        using Terminal::g_Terminal;
        using Terminal::Terminal;

        g_Terminal.WriteString("Interrupt ", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.interrupt, 10, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteString(" called!", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        if (regs.error < sizeof(g_ISRExceptions) / sizeof(g_ISRExceptions[0])) {
            g_Terminal.WriteString("\r\nException: ", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
            g_Terminal.WriteString(g_ISRExceptions[regs.error], Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        }
        g_Terminal.WriteString("\r\nCode: 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.error, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);

        g_Terminal.WriteString("\r\nRegisters:", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteString("\r\nEAX = 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.eax, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteString("\r\nEBX = 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.ebx, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteString("\r\nECX = 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.ecx, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteString("\r\nEDX = 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.edx, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteString("\r\nESI = 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.esi, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteString("\r\nEDI = 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.edi, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteString("\r\nEBP = 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.ebp, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteString("\r\nESP = 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.esp, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        
        g_Terminal.WriteString("\r\nCS = 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.cs, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteString("\r\nDS = 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.ds, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        
        g_Terminal.WriteString("\r\nEIP = 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.eip, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteString("\r\nEFLAGS = 0x", Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
        g_Terminal.WriteNumber(regs.eflags, 16, Terminal::VGAColor::VGA_COLOR_LIGHT_RED);
    }
}