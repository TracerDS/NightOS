#include <screen.hpp>
#include <driver.hpp>

// Declaration of private functions
unsigned short get_cursor_offset();
void set_cursor_offset(unsigned short offset);
unsigned short print_char(char c, unsigned char col, unsigned char row, char attr);
unsigned short get_offset(unsigned short x, unsigned short y);
unsigned char get_offset_x(unsigned short offset);
unsigned char get_offset_y(unsigned short offset);

// Private kernel functions

unsigned short get_cursor_offset() {    
    asm_write_byte(REG_SCREEN_CTRL, 14);
    unsigned short offset = asm_read_byte(REG_SCREEN_DATA) << 8;
    
    asm_write_byte(REG_SCREEN_CTRL, 15);
    offset += asm_read_byte(REG_SCREEN_DATA);

    return offset * 2;
}


void set_cursor_offset(unsigned short offset) {
    // Similar to get_cursor_offset, but instead of reading we write data
    offset /= 2;
    asm_write_byte(REG_SCREEN_CTRL, 14);
    asm_write_byte(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    asm_write_byte(REG_SCREEN_CTRL, 15);
    asm_write_byte(REG_SCREEN_DATA, (unsigned char)(offset & 0xFF));
}

unsigned short get_offset(unsigned short x, unsigned short y){
    return 2 * (y * MAX_X + x);
}
unsigned char get_offset_y(unsigned short offset){
    return offset / (2 * MAX_X);
}
unsigned char get_offset_x(unsigned short offset){
    return (offset - (get_offset_y(offset) * 2 * MAX_X))/2;
}

unsigned short print(char c, short x, short y, char attr){
    unsigned short offset;
    if(x < 0 || y < 0) offset = get_cursor_offset();
    else offset = get_offset(x, y);

    if(x >= MAX_X || y >= MAX_Y){
        VGA_MEM[2 * MAX_X * MAX_Y - 2] = 'E';
        VGA_MEM[2 * MAX_X * MAX_Y - 1] = 0x04;
        return 0;
    }

    if (c == '\r') {
        offset = get_offset(0,get_offset_x(offset));
    }else if(c == '\n') {
        offset = get_offset(get_offset_x(offset), y+1);
    } else {
        VGA_MEM[offset] = c;
        VGA_MEM[offset + 1] = 0x02;
    }
    offset = get_offset(get_offset_x(offset) + 1, get_offset_y(offset));
    set_cursor_offset(offset);
    return offset;
}
unsigned short print(const char* str, short x, short y, char attr){
    unsigned short offset;

    for(unsigned int i=0; str[i] != 0; i++){
        offset = print(str[i],x,y,attr);
        x = get_offset_x(offset);
        y = get_offset_y(offset);
    }

    return offset;
}

void clear_screen() {
    unsigned short screen_size = MAX_X * MAX_Y;
    unsigned short i;

    for (i = 0; i < screen_size; i++) {
        VGA_MEM[i*2] = ' ';
        VGA_MEM[i*2+1] = 0x0f;
    }
    set_cursor_offset(get_offset(0, 0));
}
int print_char(char c, short x, short y, char attr) {
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    if (!attr) attr = 0x0f;

    // Error control: print a red 'E' if the coords aren't right 
    if (x >= MAX_X || y >= MAX_Y) {
        vidmem[2*(MAX_X)*(MAX_Y)-2] = 'E';
        vidmem[2*(MAX_X)*(MAX_Y)-1] = 0x04;
        return get_offset(x, y);
    }

    unsigned short offset;
    if (x >= 0 && y >= 0) offset = get_offset(x, y);
    else offset = get_cursor_offset();

    if (c == '\n') {
        y = get_offset_y(offset);
        offset = get_offset(0, y+1);
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }

    set_cursor_offset(offset);
    return offset;
}


// Public Kernel API functions


// Print a message on the specified location
// If x, y, are negative, we will use the current offset
 
void kprint_at(const char* message, short x, short y) {
    // Set cursor if col/row are negative 
    int offset;
    if (x >= 0 && y >= 0)
        offset = get_offset(x, y);
    else {
        offset = get_cursor_offset();
        y = get_offset_x(offset);
        x = get_offset_y(offset);
    }

    // Loop through message and print it 
    unsigned int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], x, y, 0x0f);
        // Compute row/col for next iteration 
        x = get_offset_x(offset);
        y = get_offset_y(offset);
    }
}

void kprint(const char* message) {
    kprint_at(message, -1, -1);
}