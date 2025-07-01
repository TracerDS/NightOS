#include <tests.hpp>
#include <io.hpp>

namespace Tests {
    namespace IO {
        void Test() noexcept {
            namespace IO = ::IO;

            char fmt01 = 'A';
            auto fmt02 = "Hello, World!";
            auto fmt03 = 12345;
            auto fmt04 = 54321;
            auto fmt05 = 01234567;
            auto fmt06 = 0x1234ABCD;
            auto fmt07 = 1234567890123U;
            auto fmt08 = 3.14159;
            auto fmt09 = 1.23456;
            auto fmt10 = 5.43213;
            auto fmt11 = 8.67432;
            auto fmt12 = 0xDEADBEEF;

            IO::kprintf("Testing kprintf with various formats:\r\n");
            IO::kprintf("%%hhc ('A')               : %hhc\r\n",   fmt01);
            IO::kprintf("%%hhs (\"Hello, World!\")   : %hhs\r\n", fmt02);
            IO::kprintf("%%hhd (12345)             : %hhd\r\n",   fmt03);
            IO::kprintf("%%hhi (54321)             : %hhi\r\n",   fmt04);
            IO::kprintf("%%hho (01234567)          : %hho\r\n",   fmt05);
            IO::kprintf("%%hhx (0x1234ABCD)        : %hhx\r\n",   fmt06);
            IO::kprintf("%%hhu (1234567890123U)    : %hhu\r\n",   fmt07);
            IO::kprintf("%%hhf (3.14159)           : %hhf\r\n",   fmt08);
            IO::kprintf("%%hhe (1.23456)           : %hhf\r\n",   fmt09);
            IO::kprintf("%%hha (5.43213)           : %hhf\r\n",   fmt10);
            IO::kprintf("%%hhg (8.67432)           : %hhf\r\n",   fmt11);
            IO::kprintf("%%hhp (0xDEADBEEF)        : %hhp\r\n",   fmt12);
            IO::kprintf("-----------------------------------------\r\n");
            IO::kprintf("%%hc ('A')               : %hc\r\n",   fmt01);
            IO::kprintf("%%hs (\"Hello, World!\")   : %hs\r\n", fmt02);
            IO::kprintf("%%hd (12345)             : %hd\r\n",   fmt03);
            IO::kprintf("%%hi (54321)             : %hi\r\n",   fmt04);
            IO::kprintf("%%ho (01234567)          : %ho\r\n",   fmt05);
            IO::kprintf("%%hx (0x1234ABCD)        : %hx\r\n",   fmt06);
            IO::kprintf("%%hu (1234567890123U)    : %hu\r\n",   fmt07);
            IO::kprintf("%%hf (3.14159)           : %hf\r\n",   fmt08);
            IO::kprintf("%%he (1.23456)           : %hf\r\n",   fmt09);
            IO::kprintf("%%ha (5.43213)           : %hf\r\n",   fmt10);
            IO::kprintf("%%hg (8.67432)           : %hf\r\n",   fmt11);
            IO::kprintf("%%hp (0xDEADBEEF)        : %hp\r\n",   fmt12);
            IO::kprintf("-----------------------------------------\r\n");
            IO::kprintf("%%c ('A')               : %c\r\n",   fmt01);
            IO::kprintf("%%s (\"Hello, World!\")   : %s\r\n", fmt02);
            IO::kprintf("%%d (12345)             : %d\r\n",   fmt03);
            IO::kprintf("%%i (54321)             : %i\r\n",   fmt04);
            IO::kprintf("%%o (01234567)          : %o\r\n",   fmt05);
            IO::kprintf("%%x (0x1234ABCD)        : %x\r\n",   fmt06);
            IO::kprintf("%%u (1234567890123U)    : %u\r\n",   fmt07);
            IO::kprintf("%%f (3.14159)           : %f\r\n",   fmt08);
            IO::kprintf("%%e (1.23456)           : %f\r\n",   fmt09);
            IO::kprintf("%%a (5.43213)           : %f\r\n",   fmt10);
            IO::kprintf("%%g (8.67432)           : %f\r\n",   fmt11);
            IO::kprintf("%%p (0xDEADBEEF)        : %p\r\n",   fmt12);
            IO::kprintf("-----------------------------------------\r\n");
            IO::kprintf("%%lc ('A')               : %lc\r\n",   fmt01);
            IO::kprintf("%%ls (\"Hello, World!\")   : %ls\r\n", fmt02);
            IO::kprintf("%%ld (12345)             : %ld\r\n",   fmt03);
            IO::kprintf("%%li (54321)             : %li\r\n",   fmt04);
            IO::kprintf("%%lo (01234567)          : %lo\r\n",   fmt05);
            IO::kprintf("%%lx (0x1234ABCD)        : %lx\r\n",   fmt06);
            IO::kprintf("%%lu (1234567890123U)    : %lu\r\n",   fmt07);
            IO::kprintf("%%lf (3.14159)           : %lf\r\n",   fmt08);
            IO::kprintf("%%le (1.23456)           : %lf\r\n",   fmt09);
            IO::kprintf("%%la (5.43213)           : %lf\r\n",   fmt10);
            IO::kprintf("%%lg (8.67432)           : %lf\r\n",   fmt11);
            IO::kprintf("%%lp (0xDEADBEEF)        : %lp\r\n",   fmt12);
            IO::kprintf("-----------------------------------------\r\n");
            IO::kprintf("%%llc ('A')               : %llc\r\n",   fmt01);
            IO::kprintf("%%lls (\"Hello, World!\")   : %lls\r\n", fmt02);
            IO::kprintf("%%lld (12345)             : %lld\r\n",   fmt03);
            IO::kprintf("%%lli (54321)             : %lli\r\n",   fmt04);
            IO::kprintf("%%llo (01234567)          : %llo\r\n",   fmt05);
            IO::kprintf("%%llx (0x1234ABCD)        : %llx\r\n",   fmt06);
            IO::kprintf("%%llu (1234567890123U)    : %llu\r\n",   fmt07);
            IO::kprintf("%%llf (3.14159)           : %llf\r\n",   fmt08);
            IO::kprintf("%%lle (1.23456)           : %llf\r\n",   fmt09);
            IO::kprintf("%%lla (5.43213)           : %llf\r\n",   fmt10);
            IO::kprintf("%%llg (8.67432)           : %llf\r\n",   fmt11);
            IO::kprintf("%%llp (0xDEADBEEF)        : %llp\r\n",   fmt12);
            IO::kprintf("-----------------------------------------\r\n");
            IO::kprintf("%%Lc ('A')               : %Lc\r\n",   fmt01);
            IO::kprintf("%%Ls (\"Hello, World!\")   : %Ls\r\n", fmt02);
            IO::kprintf("%%Ld (12345)             : %Ld\r\n",   fmt03);
            IO::kprintf("%%Li (54321)             : %Li\r\n",   fmt04);
            IO::kprintf("%%Lo (01234567)          : %Lo\r\n",   fmt05);
            IO::kprintf("%%Lx (0x1234ABCD)        : %Lx\r\n",   fmt06);
            IO::kprintf("%%Lu (1234567890123U)    : %Lu\r\n",   fmt07);
            IO::kprintf("%%Lf (3.14159)           : %Lf\r\n",   fmt08);
            IO::kprintf("%%Le (1.23456)           : %Lf\r\n",   fmt09);
            IO::kprintf("%%La (5.43213)           : %Lf\r\n",   fmt10);
            IO::kprintf("%%Lg (8.67432)           : %Lf\r\n",   fmt11);
            IO::kprintf("%%Lp (0xDEADBEEF)        : %Lp\r\n",   fmt12);
            IO::kprintf("-----------------------------------------\r\n");
        }
    }
    void RunTests() noexcept {
        IO::Test();
    }
}