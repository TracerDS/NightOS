#define VIDEO_ADDRESS 0xb8000
#define VGA_MEM ((char*) VIDEO_ADDRESS)
#define MAX_X 80
#define MAX_Y 25

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

/* Public kernel API */
void clear_screen();
void kprint_at(const char* message, short x, short y);
void kprint(const char* message);
unsigned short print(char c, short x, short y, char attr);
unsigned short print(const char* str, short x, short y, char attr);