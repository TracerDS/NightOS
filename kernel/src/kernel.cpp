#include "../../drivers/include/driver.hpp"

#define VGA_OFFSET ((char*) 0xB8000)
static unsigned int __current_cursor_position = 0;

static void WriteCharacter(unsigned char c, unsigned char forecolour, unsigned char backcolour, int x, int y) {
    unsigned short attrib = (backcolour << 4) | (forecolour & 0x0F);
    char* where;
    where = VGA_OFFSET + (y * 80 + x);
    *where = c | (attrib << 8);
}
static void putChar(char c, int offset){
    VGA_OFFSET[offset] = c;
    VGA_OFFSET[offset+1] = 0x0f;
}
static void print(char c){
    putChar(c, __current_cursor_position);
    __current_cursor_position += 2;
}
static void print(char const* str){
    for(unsigned int i=0;str[i]!='\0';i++){
        print(str[i]);
    }
}

void main() {
    /* Screen cursor position: ask VGA control register (0x3d4) for bytes
     * 14 = high byte of cursor and 15 = low byte of cursor. */
    asm_write_byte(0x3d4, 0xE); /* Requesting byte 14: high byte of cursor pos */
    /* Data is returned in VGA data register (0x3d5) */
    unsigned char position = asm_read_byte(0x3d5);
    position = position << 8; /* high byte */

    asm_write_byte(0x3d4, 0xF); /* requesting low byte */
    position += asm_read_byte(0x3d5);

    /* VGA 'cells' consist of the character and its control data
     * e.g. 'white on black background', 'red text on white bg', etc */
    int offset_from_vga = position * 2;

    for(int i=0;i<1024;i++)
        putChar('\0', i*2);
    
    print('a');
    print('b');
    print("cdef");
}