#include <descriptors/isr.hpp>
#include <descriptors/idt.hpp>
#include <descriptors/gdt.hpp>
#include <descriptors/descriptors.hpp>
#include <io.hpp>

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
        using Terminal::Terminal;
        IO::kprintf_color("\r\nISR Handler %d called!\r\n",
            Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
            Terminal::VGAColor::VGA_COLOR_BLACK,
            regs.interrupt
        );

        if (regs.error < sizeof(g_ISRExceptions) / sizeof(g_ISRExceptions[0])) {
            IO::kprintf_color("Exception: %s\r\n",
                Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
                Terminal::VGAColor::VGA_COLOR_BLACK,
                g_ISRExceptions[regs.error]
            );
        }
        IO::kprintf_color("Code: 0x%X\r\n",
            Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
            Terminal::VGAColor::VGA_COLOR_BLACK,
            regs.error
        );

        IO::kprintf_color(
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
            "  EIP    = 0x%X\r\n"
            "  EFLAGS = 0x%X\r\n",
            Terminal::VGAColor::VGA_COLOR_LIGHT_RED,
            Terminal::VGAColor::VGA_COLOR_BLACK,
            regs.eax,
            regs.ebx,
            regs.ecx,
            regs.edx,
            regs.esi,
            regs.edi,
            regs.ebp,
            regs.esp,
            regs.cs,
            regs.ds,
            regs.eip,
            regs.eflags
        );
    }

    void ISR_Initialize() {
        ISR_RegisterHandler(0, Handlers::ISR0);
        ISR_RegisterHandler(1, Handlers::ISR1);
        ISR_RegisterHandler(2, Handlers::ISR2);
        ISR_RegisterHandler(3, Handlers::ISR3);
        ISR_RegisterHandler(4, Handlers::ISR4);
        ISR_RegisterHandler(5, Handlers::ISR5);
        ISR_RegisterHandler(6, Handlers::ISR6);
        ISR_RegisterHandler(7, Handlers::ISR7);
        ISR_RegisterHandler(8, Handlers::ISR8);
        ISR_RegisterHandler(9, Handlers::ISR9);
        ISR_RegisterHandler(10, Handlers::ISR10);
        ISR_RegisterHandler(11, Handlers::ISR11);
        ISR_RegisterHandler(12, Handlers::ISR12);
        ISR_RegisterHandler(13, Handlers::ISR13);
        ISR_RegisterHandler(14, Handlers::ISR14);
        ISR_RegisterHandler(15, Handlers::ISR15);
        ISR_RegisterHandler(16, Handlers::ISR16);
        ISR_RegisterHandler(17, Handlers::ISR17);
        ISR_RegisterHandler(18, Handlers::ISR18);
        ISR_RegisterHandler(19, Handlers::ISR19);
        ISR_RegisterHandler(20, Handlers::ISR20);
        ISR_RegisterHandler(21, Handlers::ISR21);
        ISR_RegisterHandler(22, Handlers::ISR22);
        ISR_RegisterHandler(23, Handlers::ISR23);
        ISR_RegisterHandler(24, Handlers::ISR24);
        ISR_RegisterHandler(25, Handlers::ISR25);
        ISR_RegisterHandler(26, Handlers::ISR26);
        ISR_RegisterHandler(27, Handlers::ISR27);
        ISR_RegisterHandler(28, Handlers::ISR28);
        ISR_RegisterHandler(29, Handlers::ISR29);
        ISR_RegisterHandler(30, Handlers::ISR30);
        ISR_RegisterHandler(31, Handlers::ISR31);
        ISR_RegisterHandler(32, Handlers::ISR32);
        ISR_RegisterHandler(33, Handlers::ISR33);
        ISR_RegisterHandler(34, Handlers::ISR34);
        ISR_RegisterHandler(35, Handlers::ISR35);
        ISR_RegisterHandler(36, Handlers::ISR36);
        ISR_RegisterHandler(37, Handlers::ISR37);
        ISR_RegisterHandler(38, Handlers::ISR38);
        ISR_RegisterHandler(39, Handlers::ISR39);
        ISR_RegisterHandler(40, Handlers::ISR40);
        ISR_RegisterHandler(41, Handlers::ISR41);
        ISR_RegisterHandler(42, Handlers::ISR42);
        ISR_RegisterHandler(43, Handlers::ISR43);
        ISR_RegisterHandler(44, Handlers::ISR44);
        ISR_RegisterHandler(45, Handlers::ISR45);
        ISR_RegisterHandler(46, Handlers::ISR46);
        ISR_RegisterHandler(47, Handlers::ISR47);
        ISR_RegisterHandler(48, Handlers::ISR48);
        ISR_RegisterHandler(49, Handlers::ISR49);
        ISR_RegisterHandler(50, Handlers::ISR50);
        ISR_RegisterHandler(51, Handlers::ISR51);
        ISR_RegisterHandler(52, Handlers::ISR52);
        ISR_RegisterHandler(53, Handlers::ISR53);
        ISR_RegisterHandler(54, Handlers::ISR54);
        ISR_RegisterHandler(55, Handlers::ISR55);
        ISR_RegisterHandler(56, Handlers::ISR56);
        ISR_RegisterHandler(57, Handlers::ISR57);
        ISR_RegisterHandler(58, Handlers::ISR58);
        ISR_RegisterHandler(59, Handlers::ISR59);
        ISR_RegisterHandler(60, Handlers::ISR60);
        ISR_RegisterHandler(61, Handlers::ISR61);
        ISR_RegisterHandler(62, Handlers::ISR62);
        ISR_RegisterHandler(63, Handlers::ISR63);
        ISR_RegisterHandler(64, Handlers::ISR64);
        ISR_RegisterHandler(65, Handlers::ISR65);
        ISR_RegisterHandler(66, Handlers::ISR66);
        ISR_RegisterHandler(67, Handlers::ISR67);
        ISR_RegisterHandler(68, Handlers::ISR68);
        ISR_RegisterHandler(69, Handlers::ISR69);
        ISR_RegisterHandler(70, Handlers::ISR70);
        ISR_RegisterHandler(71, Handlers::ISR71);
        ISR_RegisterHandler(72, Handlers::ISR72);
        ISR_RegisterHandler(73, Handlers::ISR73);
        ISR_RegisterHandler(74, Handlers::ISR74);
        ISR_RegisterHandler(75, Handlers::ISR75);
        ISR_RegisterHandler(76, Handlers::ISR76);
        ISR_RegisterHandler(77, Handlers::ISR77);
        ISR_RegisterHandler(78, Handlers::ISR78);
        ISR_RegisterHandler(79, Handlers::ISR79);
        ISR_RegisterHandler(80, Handlers::ISR80);
        ISR_RegisterHandler(81, Handlers::ISR81);
        ISR_RegisterHandler(82, Handlers::ISR82);
        ISR_RegisterHandler(83, Handlers::ISR83);
        ISR_RegisterHandler(84, Handlers::ISR84);
        ISR_RegisterHandler(85, Handlers::ISR85);
        ISR_RegisterHandler(86, Handlers::ISR86);
        ISR_RegisterHandler(87, Handlers::ISR87);
        ISR_RegisterHandler(88, Handlers::ISR88);
        ISR_RegisterHandler(89, Handlers::ISR89);
        ISR_RegisterHandler(90, Handlers::ISR90);
        ISR_RegisterHandler(91, Handlers::ISR91);
        ISR_RegisterHandler(92, Handlers::ISR92);
        ISR_RegisterHandler(93, Handlers::ISR93);
        ISR_RegisterHandler(94, Handlers::ISR94);
        ISR_RegisterHandler(95, Handlers::ISR95);
        ISR_RegisterHandler(96, Handlers::ISR96);
        ISR_RegisterHandler(97, Handlers::ISR97);
        ISR_RegisterHandler(98, Handlers::ISR98);
        ISR_RegisterHandler(99, Handlers::ISR99);
        ISR_RegisterHandler(100, Handlers::ISR100);
        ISR_RegisterHandler(101, Handlers::ISR101);
        ISR_RegisterHandler(102, Handlers::ISR102);
        ISR_RegisterHandler(103, Handlers::ISR103);
        ISR_RegisterHandler(104, Handlers::ISR104);
        ISR_RegisterHandler(105, Handlers::ISR105);
        ISR_RegisterHandler(106, Handlers::ISR106);
        ISR_RegisterHandler(107, Handlers::ISR107);
        ISR_RegisterHandler(108, Handlers::ISR108);
        ISR_RegisterHandler(109, Handlers::ISR109);
        ISR_RegisterHandler(110, Handlers::ISR110);
        ISR_RegisterHandler(111, Handlers::ISR111);
        ISR_RegisterHandler(112, Handlers::ISR112);
        ISR_RegisterHandler(113, Handlers::ISR113);
        ISR_RegisterHandler(114, Handlers::ISR114);
        ISR_RegisterHandler(115, Handlers::ISR115);
        ISR_RegisterHandler(116, Handlers::ISR116);
        ISR_RegisterHandler(117, Handlers::ISR117);
        ISR_RegisterHandler(118, Handlers::ISR118);
        ISR_RegisterHandler(119, Handlers::ISR119);
        ISR_RegisterHandler(120, Handlers::ISR120);
        ISR_RegisterHandler(121, Handlers::ISR121);
        ISR_RegisterHandler(122, Handlers::ISR122);
        ISR_RegisterHandler(123, Handlers::ISR123);
        ISR_RegisterHandler(124, Handlers::ISR124);
        ISR_RegisterHandler(125, Handlers::ISR125);
        ISR_RegisterHandler(126, Handlers::ISR126);
        ISR_RegisterHandler(127, Handlers::ISR127);
        ISR_RegisterHandler(128, Handlers::ISR128);
        ISR_RegisterHandler(129, Handlers::ISR129);
        ISR_RegisterHandler(130, Handlers::ISR130);
        ISR_RegisterHandler(131, Handlers::ISR131);
        ISR_RegisterHandler(132, Handlers::ISR132);
        ISR_RegisterHandler(133, Handlers::ISR133);
        ISR_RegisterHandler(134, Handlers::ISR134);
        ISR_RegisterHandler(135, Handlers::ISR135);
        ISR_RegisterHandler(136, Handlers::ISR136);
        ISR_RegisterHandler(137, Handlers::ISR137);
        ISR_RegisterHandler(138, Handlers::ISR138);
        ISR_RegisterHandler(139, Handlers::ISR139);
        ISR_RegisterHandler(140, Handlers::ISR140);
        ISR_RegisterHandler(141, Handlers::ISR141);
        ISR_RegisterHandler(142, Handlers::ISR142);
        ISR_RegisterHandler(143, Handlers::ISR143);
        ISR_RegisterHandler(144, Handlers::ISR144);
        ISR_RegisterHandler(145, Handlers::ISR145);
        ISR_RegisterHandler(146, Handlers::ISR146);
        ISR_RegisterHandler(147, Handlers::ISR147);
        ISR_RegisterHandler(148, Handlers::ISR148);
        ISR_RegisterHandler(149, Handlers::ISR149);
        ISR_RegisterHandler(150, Handlers::ISR150);
        ISR_RegisterHandler(151, Handlers::ISR151);
        ISR_RegisterHandler(152, Handlers::ISR152);
        ISR_RegisterHandler(153, Handlers::ISR153);
        ISR_RegisterHandler(154, Handlers::ISR154);
        ISR_RegisterHandler(155, Handlers::ISR155);
        ISR_RegisterHandler(156, Handlers::ISR156);
        ISR_RegisterHandler(157, Handlers::ISR157);
        ISR_RegisterHandler(158, Handlers::ISR158);
        ISR_RegisterHandler(159, Handlers::ISR159);
        ISR_RegisterHandler(160, Handlers::ISR160);
        ISR_RegisterHandler(161, Handlers::ISR161);
        ISR_RegisterHandler(162, Handlers::ISR162);
        ISR_RegisterHandler(163, Handlers::ISR163);
        ISR_RegisterHandler(164, Handlers::ISR164);
        ISR_RegisterHandler(165, Handlers::ISR165);
        ISR_RegisterHandler(166, Handlers::ISR166);
        ISR_RegisterHandler(167, Handlers::ISR167);
        ISR_RegisterHandler(168, Handlers::ISR168);
        ISR_RegisterHandler(169, Handlers::ISR169);
        ISR_RegisterHandler(170, Handlers::ISR170);
        ISR_RegisterHandler(171, Handlers::ISR171);
        ISR_RegisterHandler(172, Handlers::ISR172);
        ISR_RegisterHandler(173, Handlers::ISR173);
        ISR_RegisterHandler(174, Handlers::ISR174);
        ISR_RegisterHandler(175, Handlers::ISR175);
        ISR_RegisterHandler(176, Handlers::ISR176);
        ISR_RegisterHandler(177, Handlers::ISR177);
        ISR_RegisterHandler(178, Handlers::ISR178);
        ISR_RegisterHandler(179, Handlers::ISR179);
        ISR_RegisterHandler(180, Handlers::ISR180);
        ISR_RegisterHandler(181, Handlers::ISR181);
        ISR_RegisterHandler(182, Handlers::ISR182);
        ISR_RegisterHandler(183, Handlers::ISR183);
        ISR_RegisterHandler(184, Handlers::ISR184);
        ISR_RegisterHandler(185, Handlers::ISR185);
        ISR_RegisterHandler(186, Handlers::ISR186);
        ISR_RegisterHandler(187, Handlers::ISR187);
        ISR_RegisterHandler(188, Handlers::ISR188);
        ISR_RegisterHandler(189, Handlers::ISR189);
        ISR_RegisterHandler(190, Handlers::ISR190);
        ISR_RegisterHandler(191, Handlers::ISR191);
        ISR_RegisterHandler(192, Handlers::ISR192);
        ISR_RegisterHandler(193, Handlers::ISR193);
        ISR_RegisterHandler(194, Handlers::ISR194);
        ISR_RegisterHandler(195, Handlers::ISR195);
        ISR_RegisterHandler(196, Handlers::ISR196);
        ISR_RegisterHandler(197, Handlers::ISR197);
        ISR_RegisterHandler(198, Handlers::ISR198);
        ISR_RegisterHandler(199, Handlers::ISR199);
        ISR_RegisterHandler(200, Handlers::ISR200);
        ISR_RegisterHandler(201, Handlers::ISR201);
        ISR_RegisterHandler(202, Handlers::ISR202);
        ISR_RegisterHandler(203, Handlers::ISR203);
        ISR_RegisterHandler(204, Handlers::ISR204);
        ISR_RegisterHandler(205, Handlers::ISR205);
        ISR_RegisterHandler(206, Handlers::ISR206);
        ISR_RegisterHandler(207, Handlers::ISR207);
        ISR_RegisterHandler(208, Handlers::ISR208);
        ISR_RegisterHandler(209, Handlers::ISR209);
        ISR_RegisterHandler(210, Handlers::ISR210);
        ISR_RegisterHandler(211, Handlers::ISR211);
        ISR_RegisterHandler(212, Handlers::ISR212);
        ISR_RegisterHandler(213, Handlers::ISR213);
        ISR_RegisterHandler(214, Handlers::ISR214);
        ISR_RegisterHandler(215, Handlers::ISR215);
        ISR_RegisterHandler(216, Handlers::ISR216);
        ISR_RegisterHandler(217, Handlers::ISR217);
        ISR_RegisterHandler(218, Handlers::ISR218);
        ISR_RegisterHandler(219, Handlers::ISR219);
        ISR_RegisterHandler(220, Handlers::ISR220);
        ISR_RegisterHandler(221, Handlers::ISR221);
        ISR_RegisterHandler(222, Handlers::ISR222);
        ISR_RegisterHandler(223, Handlers::ISR223);
        ISR_RegisterHandler(224, Handlers::ISR224);
        ISR_RegisterHandler(225, Handlers::ISR225);
        ISR_RegisterHandler(226, Handlers::ISR226);
        ISR_RegisterHandler(227, Handlers::ISR227);
        ISR_RegisterHandler(228, Handlers::ISR228);
        ISR_RegisterHandler(229, Handlers::ISR229);
        ISR_RegisterHandler(230, Handlers::ISR230);
        ISR_RegisterHandler(231, Handlers::ISR231);
        ISR_RegisterHandler(232, Handlers::ISR232);
        ISR_RegisterHandler(233, Handlers::ISR233);
        ISR_RegisterHandler(234, Handlers::ISR234);
        ISR_RegisterHandler(235, Handlers::ISR235);
        ISR_RegisterHandler(236, Handlers::ISR236);
        ISR_RegisterHandler(237, Handlers::ISR237);
        ISR_RegisterHandler(238, Handlers::ISR238);
        ISR_RegisterHandler(239, Handlers::ISR239);
        ISR_RegisterHandler(240, Handlers::ISR240);
        ISR_RegisterHandler(241, Handlers::ISR241);
        ISR_RegisterHandler(242, Handlers::ISR242);
        ISR_RegisterHandler(243, Handlers::ISR243);
        ISR_RegisterHandler(244, Handlers::ISR244);
        ISR_RegisterHandler(245, Handlers::ISR245);
        ISR_RegisterHandler(246, Handlers::ISR246);
        ISR_RegisterHandler(247, Handlers::ISR247);
        ISR_RegisterHandler(248, Handlers::ISR248);
        ISR_RegisterHandler(249, Handlers::ISR249);
        ISR_RegisterHandler(250, Handlers::ISR250);
        ISR_RegisterHandler(251, Handlers::ISR251);
        ISR_RegisterHandler(252, Handlers::ISR252);
        ISR_RegisterHandler(253, Handlers::ISR253);
        ISR_RegisterHandler(254, Handlers::ISR254);
        ISR_RegisterHandler(255, Handlers::ISR255);
    }
}