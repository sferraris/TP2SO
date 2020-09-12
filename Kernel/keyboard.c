#include <keyboard.h>

extern char getKey();
int wBuffer = 0;
int shift=0;
int alt=0;
int registerBuffer[15];

void changeKeyboard()
{
    wBuffer = 1-wBuffer;
}

struct wKeyboard 
{
    char buffer[50];
    int readCurrent;
    int writeCurrent;
};

struct wKeyboard kwindows[2]; 

void initKeyboardBuffer(char *buffer)
{
    for ( int i = 0; i < 50; i++)
        buffer[i] = 0;
}

void initKeyboardStruct(struct wKeyboard *wind)
{
    wind->readCurrent = 0;
    wind->writeCurrent = 0;
    initKeyboardBuffer(wind->buffer);
}

void initializeKeyboard()
{
    struct wKeyboard leftWindow;
    struct wKeyboard rightWindow;
    initKeyboardStruct(&leftWindow);
    initKeyboardStruct(&rightWindow);
    kwindows[0] = leftWindow;
    kwindows[1] = rightWindow;
}

static char keySet[87][3]=
{{0,0,0},{27,27,27},{'1','!','|'},{'2','"','@'},{'3',0,'#'},{'4','$','~'},{'5','%',0},{'6','&',0},{'7','/',0},{'8','(',0},{'9',')',0},
{'0','=',0},{'\'','?',0},{0,0,0},{8,8,8},{9,9,9},{'q','Q',0},{'w','W',0},{'e','E',0},{'r','R',0},{'t','T',0},{'y','Y',0},{'u','U',0},
{'i','I',0},{'o','O',0},{'p','P',0},{'`','^','['},{'+','*',']'},{'\n','\n','\n'},{0,0,0},{'a','A',0},{'s','S',5},{'d','D',0},
{'f','F',0},{'g','G',0},{'h','H',0},{'j','J',0},{'k','K',0},{'l','L',0},{0,0,0},{0,0,'{'},{0,0,'\\'},{0,0,0},{0,0,'}'},{'z','Z',0},
{'x','X',0},{'c','C',0},{'v','V',0},{'b','B',0},{'n','N',0},{'m','M',0},{',',';',0},{'.',':',0},{'-','_',0},{0,0,0},{'*',0,0},{0,0,0},
{' ',' ',0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{'7',0,0},
{'8',0,0},{'9',0,0},{'-',0,0},{'4',0,0},{'5',0,0},{'6',0,0},{'+',0,0},{'1',0,0},{'2',0,0},{'3',0,0},{'0',0,0},{'.',0,0},{0,0,0},
{0,0,0},{'<','>',0}};

void saveChar(char c);

void key_handler()
{
    char key = getKey();
    switch(key)
    {
        case 42:case 54:case 58:
        case 18446744073709551530:
        case 18446744073709551542:shift=1-shift;break;
        case 18446744073709551544:case 56:alt=2-alt;break; 
    }
    int special = (alt>shift)?alt:shift;
    if (key>0 && keySet[key][special])
    {
        if ( keySet[key][special] == 5)
            saveRegisters(registerBuffer);
        else
            saveChar(keySet[key][special]);
    }
}

int* getRegisters()
{
    return registerBuffer;
}

void saveChar(char c)
{
    kwindows[wBuffer].buffer[kwindows[wBuffer].writeCurrent] = c;
    if ( kwindows[wBuffer].writeCurrent == 49)
        kwindows[wBuffer].writeCurrent = 0;
    else
        kwindows[wBuffer].writeCurrent ++;
}

char read_key()
{
    while (!kwindows[wBuffer].buffer[kwindows[wBuffer].readCurrent])
        _hlt();
    char aux = kwindows[wBuffer].buffer[kwindows[wBuffer].readCurrent];
    kwindows[wBuffer].buffer[kwindows[wBuffer].readCurrent] = 0;
    if ( kwindows[wBuffer].readCurrent == 49)
        kwindows[wBuffer].readCurrent = 0;
    else
        kwindows[wBuffer].readCurrent ++;
    return aux;
}