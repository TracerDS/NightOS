#include "../../drivers/include/screen.hpp"

extern "C" char _BSS_START_, _BSS_END_;

void main() {
    for (char* ptr = &_BSS_START_; ptr < &_BSS_END_; ptr++) {
        *ptr = 0;
    }
    //clear_screen();
    for(;;);
    //print("Hello, World",0,0,0);
    /*
    print('H',0,0,0);
    print('e',1,0,0);
    print('l',2,0,0);
    print('l',3,0,0);
    print('o',4,0,0);
    */
    /*
    kprint_at("This text spans multiple lines", 75, 10);
    kprint_at("There is a line\nbreak", 0, 20);
    kprint("There is a line\nbreak");
    kprint_at("What happens when we run out of space?", 45, 24);
    */
}