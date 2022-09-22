//#include "screen.h"
//#include "ports.h"
#include <driver.hpp>
#include <screen.hpp>

/* Declaration of private functions */
unsigned short get_cursor_offset();
void set_cursor_offset(unsigned short offset);
unsigned short print(char c, unsigned short x, unsigned short y, char attr);
unsigned short get_offset(unsigned short x, unsigned short y);
unsigned short get_offset_x(unsigned short offset);
unsigned short get_offset_y(unsigned short offset);


/*
* Private kernel functions
*/

/**
 * Innermost print function for our kernel, directly accesses the video memory 
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */

unsigned short print(char c, unsigned short x, unsigned short y, char attr = 0){
    if(!attr) attr = 0x0f;

    if(x >= MAX_X || y >= MAX_Y) {
        VGA_MEM[2 * MAX_COLS * MAX_ROWS - 2] = 'E';
        VGA_MEM[2 * MAX_COLS * MAX_ROWS - 1] = 0xf4;
        return get_offset(x, y);
    }

    unsigned short offset;
    if(x >= 0 && y >= 0) offset = get_offset(x, y);
    else offset = get_cursor_offset();
    
    if (c == '\r') {
        offset = get_offset(0, get_offset_y(offset));
    } else if (c == '\n'){
        offset = get_offset(get_offset_x(offset), get_offset_y(offset)+1);
    }else {
        VGA_MEM[offset] = c;
        VGA_MEM[offset+1] = attr;
        offset += 2;
    }

    set_cursor_offset(offset);
    return offset;
}

unsigned short get_cursor_offset() {
    /* Use the VGA ports to get the current cursor position
     * 1. Ask for high byte of the cursor offset (data 14)
     * 2. Ask for low byte (data 15)
     */
    asm_write_byte(REG_SCREEN_CTRL, 14);
    int offset = asm_read_byte(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    asm_write_byte(REG_SCREEN_CTRL, 15);
    offset += asm_read_byte(REG_SCREEN_DATA);

    return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(unsigned short offset) {
    /* Similar to get_cursor_offset, but instead of reading we write data */
    offset /= 2;
    asm_write_byte(REG_SCREEN_CTRL, 14);
    asm_write_byte(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    asm_write_byte(REG_SCREEN_CTRL, 15);
    asm_write_byte(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
    for (unsigned short i = 0; i < MAX_X * MAX_Y; i++) {
        VGA_MEM[i*2] = 0;
        VGA_MEM[i*2+1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}

unsigned short get_offset(unsigned short x, unsigned short y) {
    return 2 * (y * MAX_X + x);
}
unsigned short get_offset_x(unsigned short offset) {
    return (offset - (get_offset_x(offset)*2*MAX_Y))/2;
}
unsigned short get_offset_y(unsigned short offset) {
    return offset / (2 * MAX_X);
}


/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void kprint_at(const char* str, short x, short y) {
    /* Set cursor if col/row are negative */
    unsigned short offset;
    if (x >= 0 && y >= 0)
        offset = get_offset(x, y);
    else {
        offset = get_cursor_offset();
        x = get_offset_x(x);
        y = get_offset_y(y);
    }

    /* Loop through message and print it */
    unsigned short i = 0;
    while (str[i] != 0) {
        offset = print(str[i++], x, y, 0x0f);
        /* Compute row/col for next iteration */
        x = get_offset_x(offset);
        y = get_offset_y(offset);
    }
}

void kprint(const char* str) {
    kprint_at(str, -1, -1);
}