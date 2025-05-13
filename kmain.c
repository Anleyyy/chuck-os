const unsigned char BLACK = 0x0;
const unsigned char WHITE = 0xF;

void print(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    char *fb = (char *) 0x000B8000;
    fb[i*2] = c;
    fb[i * 2 + 1] = (fg & 0x0F) | ((bg & 0x0F) << 4);
}



void print_str(unsigned int j, char *str, unsigned char fg, unsigned char bg)
{
    
    unsigned int i = 0;

    while (str[i] != '\0') {
        if (str[i] == 0x7E){
            char a = 0;
            while ((j % 80 != 0) || (a == 0)){
                a = 1;
                print(j,' ',fg,bg);
                j++;
            }
            j--;
        }
        else{
            print(j, str[i], fg, bg);
        }
        
        j++;
        i++;
    }
}

void clear()
{
    print_str(0,"~~~~~~~~~~~~~~",WHITE,BLACK);
}

void unicorn(unsigned char fg, unsigned char bg)
{
    print_str(50,"Hello world !~Ca marche !",fg,bg);
    clear();
}

