#include "lib_c.h"
#include "kmain.h"
#include "paging.h"

extern unsigned int kernel_page_directory[1024];
extern unsigned int white[1];
char pages[NB_PAGES];

void change_page(unsigned char value, unsigned int i) {
    pages[i] = value;
}

char get_page(unsigned int i) {
    return pages[i];
}

void unicorn(unsigned char fg, unsigned char bg)
{
    clear_console();
    init_pages();
}
