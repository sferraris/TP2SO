#include <shell.h>

char shellBuffer[100] = {0};
char * commandArray[COMMANDS]= {"help", "time", "cpudata", "cputemp", "printmem","inforeg", "zerotest", "opcodetest","ps","nice","block"};
int shellPos = 0;
char* regNames[15] = {"RAX: ", "RBX: ","RCX: ","RDX: ","RBP: ","RDI: ","RSI: ","R8: ","R9: ","R10: ","R11: ","R12: ","R13: ","R14: ","R15: ",};

void printMem();
void processCommand();
int matchArray();

int matchArray() {
    int j;
    for ( j = 0; j < COMMANDS; j++) {
        if(strcmpParams(shellBuffer,commandArray[j]))
            return j;
    }
    return -1;
}

void shellBackSpace() {
    if (shellPos) {
        shellBuffer[shellPos] = 0;
        shellPos--;
        putChar(BACKSPACE);
    }
}

void shellCE() {
    while(shellPos)
        shellBackSpace();
}

void help() {
    printRed("time: ");printf("Displays current time\n");
    printRed("cpudata: ");printf("Displays vendor, brand and model of your cpu\n");
    printRed("cputemp: ");printf("Displays the temperature of your cpu\n");
    printRed("printmem: ");printf("Receives a pointer in hexa and Displays 32 byte memory dump from that pointer\n");
    printRed("inforeg: ");printf("Displays register values previously saved with alt+s\n");
    printRed("zerotest: ");printf("Triggers exception 0\n");
    printRed("opcodetest: ");printf("Triggers exception 6\n");
    printRed("ps: ");printf("Lists all active processes\n");
    printRed("nice: ");printf("Changes priviledge level from a given process. Format: nice pri pid\n");
}

void time() {
    uint64_t hours = getHours();
    uint64_t mins = getMinutes();
    uint64_t secs = getSeconds();
    if ( hours < 10)
        putDec(0);
    putHex(hours);
    putChar(':');
    if ( mins < 10)
        putDec(0);
    putHex(mins);
    putChar(':');
    if ( secs < 10)
        putDec(0);
    putHex(secs);
    putChar('\n');
}

void cpuData() {
    char buffer1[10];
    char buffer2[64]="Function not supported\n";
    printf("Vendor: ");
    printf(getCpuVendor(buffer1));
    putChar('\n');
    printf("Brand: ");
    printf(getCpuBrand(buffer2));
    putChar('\n');
    printf("Model: ");
    putDec(getCpuModel());
    putChar('\n');
}

void cpuTemp() {
    putDec(getTemp());
    printf("C\n");
}

void printMem() {
    if (shellBuffer[9] == '0' && shellBuffer[10] == 'x') {
        uint64_t num = stringHexToInt(shellBuffer + 11);
        uint64_t buff[4];
        getmem(num, buff);
        for ( int i = 0; i < 4; i++)
            putFilledHex(buff[i]);
        putChar('\n');
    }
}

void zero_test() {
    zero_exception_creator();
}

void invalidopcode_test() {
    invalid_opcode_creator();
}

void nice() { 
    int pid;
    int pri;
    if (shellBuffer[4] == ' ' && shellBuffer[6] == ' ') {
        pri = shellBuffer[5] - '0';
        if (pri < 0 || pri > 9) {
            printf("Numero incorrecto de privilegio\n");
            return;
        }  
        pid = shellBuffer[7] - '0';
        if (pid < 0 || pid > 9) {
            printf("Numero incorrecto de ID\n");  
            return;
        }
        if (isNumberHexa(shellBuffer[8])) {
            pid = pid * 10 + (shellBuffer[8] - '0');
        }
        changePriority(pid,pri);
    }
    else
        printf("Formato incorrecto");
}

void block() {
    int pid;
    if (shellBuffer[5] == ' ') {
      pid = shellBuffer[6] - '0';
        if (pid < 0 || pid > 9) {
            printf("Numero incorrecto de ID\n");  
            return;
        }
        if (isNumberHexa(shellBuffer[7])) {
            pid = pid * 10 + (shellBuffer[7] - '0');
        }
        blockProcess(pid);
    }
}

void inforeg() {
    int* aux = receiveRegisters();
    if ( aux[5] != 0) {
        for ( int i = 0; i < 15; i++) {
            printf(regNames[i]);putHex(aux[i]);putChar('\n');
        }
    }
    else
        printf("Registers not saved\n");
}

void processCommand() {
    putChar('\n');
    int command = matchArray();
    switch(command) {
        case 0:help();break;
        case 1:time();break;
        case 2:cpuData();break;
        case 3:cpuTemp();break;
        case 4:printMem();break;
        case 5:inforeg();break;
        case 6:zero_test();break;
        case 7:invalidopcode_test();break;
        case 8:listProcesses();break;
        case 9:nice();break;
        case 10:block();break;
        default:printf("Error: command doesnt match\n"); 
    }
}

int pidPrueba=0;

void prueba1() {
    pidPrueba=getPid();
    while(1) {
        putDec(getPid());
    }
}

void initShell() {
    //printf("Corre shell");
   // sendIp();
    char * aux = malloc(1024*4*20 + 1);
   // printf("pasa primer malloc");
    //char * aux3 = malloc(1024*4*69);
    //putHex((uint64_t)aux);
    //free(aux);
    char * aux4 = malloc(1024*4*69);
   // printf("pasa segundo malloc");
   // putHex((uint64_t)aux4);
    free(aux);
    //printf("pasa primer free");
    free(aux4);
    //printf("pasa segundo free");
    while (1) { 
        putChar('>');
        char c = getChar();
        while (c != '\n' ) {
            switch(c) {
                case BACKSPACE: shellBackSpace();break;
                case TAB: shellCE();break;
                case '5': createProcess(prueba1);break;
                case '6': killProcess(pidPrueba);break;
                case '7': listProcesses();break;
                default:if ( shellPos < 100) { 
                            putChar(c);
                            shellBuffer[shellPos++] = c;
                        }
            }
            c = getChar();
        }
        shellBuffer[shellPos]=0;
        shellPos = 0;
        processCommand();
    }
}