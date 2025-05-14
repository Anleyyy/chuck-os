#include "lib_c.h"
#include "kmain.h"
#include "paging.h"

extern unsigned int kernel_page_directory[1024];
extern unsigned int white[1];

void unicorn(unsigned char fg, unsigned char bg)
{
    clear_console();
    struct output hello_world = {50,"Hello world~ Let's goooo !!",fg,bg};
    print_str(hello_world);
    print_addr(200, kernel_page_directory);
}
