#include "lib_c.h"

void print(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    char *fb = (char *) 0x000B8000;
    fb[i*2] = c;
    fb[i * 2 + 1] = (fg & 0x0F) | ((bg & 0x0F) << 4);
}

void print_str(struct output out)
{
    
    unsigned int i = 0;

    while (out.str[i] != '\0') {
        if (out.str[i] == 0x7E){
            char a = 0;
            while ((out.j % 80 != 0) || (a == 0)){
                a = 1;
                print(out.j,' ',out.fg,out.bg);
                out.j++;
            }
            out.j--;
        }
        else{
            print(out.j, out.str[i], out.fg, out.bg);
        }
        
        out.j++;
        i++;
    }
}

void clear_console()
{
    struct output clear_output = {0,"~~~~~~~~~~~~~~",WHITE,BLACK};
    print_str(clear_output);
}

void int_to_hex_str(unsigned int num, char* buffer) {
    const char* hex_chars = "0123456789ABCDEF";
    buffer[0] = '0';
    buffer[1] = 'x';
    for (int i = 0; i < 8; i++) {
        buffer[2 + i] = hex_chars[(num >> (28 - 4 * i)) & 0xF];
    }
    buffer[10] = '\0';
}

void print_addr(unsigned int i, unsigned int *ptr) {
    char buffer[11];
    unsigned int addr = (unsigned int)ptr;
    int_to_hex_str(addr, buffer);
    struct output adresse = {i,buffer,WHITE,BLACK};
    print_str(adresse);
}
