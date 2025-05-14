#define BLACK 0x0
#define WHITE 0xF

struct output {
    unsigned int j;
    char *str;
    unsigned char fg;
    unsigned short bg;
} __attribute__((packed));

void print(unsigned int i, char c, unsigned char fg, unsigned char bg);

void print_str(struct output out);

void clear_console();

void int_to_hex_str(unsigned int num, char* buffer);

void print_addr(unsigned int i, unsigned int *ptr);
