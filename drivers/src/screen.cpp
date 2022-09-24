#include <screen.hpp>
#include <driver.hpp>

// Declaration of private functions
/*
unsigned short get_cursor_offset();
void set_cursor_offset(unsigned short offset);
unsigned short print_char(char c, unsigned char col, unsigned char row, char attr);
unsigned short get_offset(short col, short row);
unsigned short get_offset_row(unsigned short offset);
unsigned short get_offset_col(unsigned short offset);
*/

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

void clear_screen(){
    //unsigned short screen_size = MAX_X * MAX_Y;
}

/*
void clear_screen() {
    unsigned short screen_size = MAX_X * MAX_Y;
    unsigned short i;

    for (i = 0; i < screen_size; i++) {
        VGA_MEM[i*2] = ' ';
        VGA_MEM[i*2+1] = 0x0f;
    }
    set_cursor_offset(get_offset(0, 0));
}
int print_char(char c, short col, short row, char attr) {
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    if (!attr) attr = 0x0f;

    // Error control: print a red 'E' if the coords aren't right 
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = 0x04;
        return get_offset(col, row);
    }

    unsigned short offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }

    set_cursor_offset(offset);
    return offset;
}

unsigned short get_cursor_offset() {
    // Use the VGA ports to get the current cursor position
    // 1. Ask for high byte of the cursor offset (data 14)
    // 2. Ask for low byte (data 15)
    
    asm_write_byte(REG_SCREEN_CTRL, 14);
    unsigned short offset = (unsigned short) asm_read_byte(REG_SCREEN_DATA) << 8; // High byte: << 8
    
    asm_write_byte(REG_SCREEN_CTRL, 15);
    offset += asm_read_byte(REG_SCREEN_DATA);

    return offset * 2; // Position * size of character cell
}

void set_cursor_offset(unsigned short offset) {
    // Similar to get_cursor_offset, but instead of reading we write data
    offset /= 2;
    asm_write_byte(REG_SCREEN_CTRL, 14);
    asm_write_byte(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    asm_write_byte(REG_SCREEN_CTRL, 15);
    asm_write_byte(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
    unsigned short screen_size = MAX_COLS * MAX_ROWS;
    unsigned short i;
    char* screen = VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = 0x0f;
    }
    set_cursor_offset(get_offset(0, 0));
}

// Public Kernel API functions


// Print a message on the specified location
// If col, row, are negative, we will use the current offset
 
void kprint_at(const char* message, short col, short row) {
    // Set cursor if col/row are negative 
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    // Loop through message and print it 
    unsigned int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, 0x0f);
        // Compute row/col for next iteration 
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void kprint(const char* message) {
    kprint_at(message, -1, -1);
}
*/