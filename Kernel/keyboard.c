#include <keyboard.h>

extern char getKey();
int shift=0;
int alt=0;
int registerBuffer[15];
char buffer[50];
int readCurrent = 0;
int writeCurrent = 0;

static char keySet[87][3] =
{{0,0,0},{0,0,0},{'1','!','|'},{'2','"','@'},{'3',0,'#'},{'4','$','~'},{'5','%',0},{'6','&',0},{'7','/',0},{'8','(',0},{'9',')',0},
{'0','=',0},{'\'','?',0},{0,0,0},{8,8,8},{9,9,9},{'q','Q',0},{'w','W',0},{'e','E',0},{'r','R',0},{'t','T',0},{'y','Y',0},{'u','U',0},
{'i','I',0},{'o','O',0},{'p','P',0},{'`','^','['},{'+','*',']'},{'\n','\n','\n'},{0,0,0},{'a','A',0},{'s','S',5},{'d','D',0},
{'f','F',0},{'g','G',0},{'h','H',0},{'j','J',0},{'k','K',0},{'l','L',0},{0,0,0},{0,0,'{'},{0,0,'\\'},{0,0,0},{0,0,'}'},{'z','Z',0},
{'x','X',0},{'c','C',0},{'v','V',0},{'b','B',0},{'n','N',0},{'m','M',0},{',',';',0},{'.',':',0},{'-','_',0},{0,0,0},{'*',0,0},{0,0,0},
{' ',' ',0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{'7',0,0},
{'8',0,0},{'9',0,0},{'-',0,0},{'4',0,0},{'5',0,0},{'6',0,0},{'+',0,0},{'1',0,0},{'2',0,0},{'3',0,0},{'0',0,0},{'.',0,0},{0,0,0},
{0,0,0},{'<','>',0}};

void saveChar(char c);

void key_handler() {
    int key = getKey();
    switch(key) {
        case 42:case 54:case 58:
        case 18446744073709551530:
        case 18446744073709551542:shift=1-shift;break;
        case 18446744073709551544:case 56:alt=2-alt;break; 
    }
    int special = (alt>shift)?alt:shift;
    if (key>0 && keySet[key][special]) {
        if (keySet[key][special] == 5)
            saveRegisters(registerBuffer);
        else {
            saveChar(keySet[key][special]);
            detectChar();
        }
    }
}

int* getRegisters() {
    return registerBuffer;
}

void saveChar(char c) {
    buffer[writeCurrent] = c;
    if (writeCurrent == 49)
        writeCurrent = 0;
    else
        writeCurrent++;
}

char read_key() {
    while (!buffer[readCurrent])
        waitForChar(getPid());
    char aux = buffer[readCurrent];
    buffer[readCurrent] = 0;
    if (readCurrent == 49)
        readCurrent = 0;
    else
        readCurrent++;
    return aux;
}