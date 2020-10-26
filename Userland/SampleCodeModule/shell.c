// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>

char shellBuffer[100] = {0};
char * commandArray[COMMANDS]= {"help", "time", "cpudata", "cputemp", "mem","inforeg","ps","nice","block", "kill","loop", "sem", "pipe", "phylo", "testmm", "testprocesses", "testprio", "testsem", "cat", "wc", "filter"};
int shellPos = 0;
char* regNames[15] = {"RAX: ", "RBX: ","RCX: ","RDX: ","RBP: ","RDI: ","RSI: ","R8: ","R9: ","R10: ","R11: ","R12: ","R13: ","R14: ","R15: "};

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
    printf("time: Displays current time\n");
    printf("cpudata:  vendor, brand and model of your cpu\n");
    printf("cputemp: Displays the temperature of your cpu\n");
    printf("mem: Receives a pointer in hexa and Displays 32 byte memory dump from that pointer\n");
    printf("inforeg: Displays register values previously saved with alt+s\n");
    printf("ps: Lists all active processes\n");
    printf("nice: Changes priviledge level from a given process. Format: nice pri pid\n");
    printf("block: blocks a process given its pid\n");
    printf("kill: kills a process given its pid\n");
    printf("loop: Prints pid after a certain amount of seconds\n");
    printf("sem: Prints the name, status and ids of the blocked processes for each active semaphore\n");
    printf("pipe: Prints the id, status and ids of the blocked processes for each active pipe\n");
    printf("cat: Prints stdin as received\n");
    printf("wc: Counts the lines from the input\n");
    printf("filter: Filters the vowels from the input\n");
    printf("phylo: Simulates the phylosopher problem\n");
    printf("testmm: Runs a mememory manager test\n");
    printf("testprocesses: Runs a processes manager test\n");
    printf("testprio: Runs a priority manager test\n");
    printf("testsem: Runs a semaphore manager test\n");
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

void nice() { 
    int pid, pri;
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

void kill() { 
    int pid; 
    if (shellBuffer[4] == ' ') { 
        pid = shellBuffer[5] - '0'; 
        if (pid < 0 || pid > 9) { 
            printf("Numero incorrecto de ID\n"); 
            return; 
        } 
        if (isNumberHexa(shellBuffer[6])) { 
            pid = pid * 10 + (shellBuffer[6] - '0'); 
        } 
        killProcess(pid); 
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

void TMM(){
    uint64_t fg = (shellBuffer[6] == '&') ? BACKGROUND : FOREGROUND;
    char * argv[] = {(char *)testMM, "Testmm", fg, STD_IN, (char *)STD_OUT};
    createProcess(5, argv);
}

void TPRO(){
    uint64_t fg = (shellBuffer[13] == '&') ? BACKGROUND : FOREGROUND;
    char * argv[] = {(char *)proTest, "testprocesses", fg, STD_IN, (char *)STD_OUT};
    createProcess(5, argv);
}

void TPRI(){
    uint64_t fg = (shellBuffer[8] == '&') ? BACKGROUND : FOREGROUND;
    char * argv[] = {(char *)priorityTest, "testprio", fg, STD_IN, (char *)STD_OUT};
    createProcess(5, argv);
}

void TSYNC(){
    uint64_t fg = (shellBuffer[7] == '&') ? BACKGROUND : FOREGROUND;
    char * argv[] = {(char *)Tsync, "testsem", fg, STD_IN, (char *)STD_OUT};
    createProcess(5, argv);
}

void writingProcess() {
    while (1) {
        char c;
        getChar(&c);
        while (c != '\n') {
            switch (c) {
                case BACKSPACE: shellBackSpace();break;
                case TAB: shellCE();break;
                default:
                    if (c == '$') {
                        write(3, "$\n", 2);
                        shellPos = 0;
                        exit();
                    }
                    if (shellPos < 100) {
                        putChar(c);
                        shellBuffer[shellPos++] = c;
                    }
            }
            getChar(&c);
        }
        putChar(c);
        shellBuffer[shellPos] = c;
        shellBuffer[shellPos+1] = 0;
        write(3, shellBuffer, shellPos+1);
        shellPos = 0;
        yield();
    }
}

void catFunction() {
    char catBuf[100];
    int size;
    do {
        size = read(2, catBuf, 100);
        write(3, catBuf, size);
    } while(catBuf[0] != '$');
    exit();
}

int cat(uint64_t input, uint64_t output, int left){
    char * argv[] = {(char *)catFunction, "cat", BACKGROUND, (char *)input, (char *)output};
    if (left == 1) {
        int p[2];
        int pipeId = pipe(p);
        argv[3] = (char *) p[0];
        int catID = createProcess(5, argv);
        char * argv2[] = {(char *)writingProcess, "wp", (char *)FOREGROUND, STD_IN, (char *)p[1]};
        createProcess(5, argv2);
        wait(catID);
        close(pipeId);
        return 0;
    }
    else
        return createProcess(5, argv);
}

void wcFunction(){
    char wcBuff[100];
    int lineCount = -1;
    do {
        lineCount++;
        read(2, wcBuff, 100);
    } while(wcBuff[0] != '$');
    wcBuff[0] = 0;
    strcat(wcBuff, "$Line count: ");
    strcat(wcBuff, dectostr(lineCount));
    strcat(wcBuff, "\n");
    write(3, wcBuff, 100);
    exit();
}

int wc(uint64_t input, uint64_t output, int left){
    char * argv[] = {(char *)wcFunction, "wc", BACKGROUND,(char *)input, (char *)output};
    if (left == 1) {
        int p[2];
        int pipeId = pipe(p);
        argv[3] = (char *)p[0];
        int wcId = createProcess(5, argv);
        char * argv2[] = {(char *)writingProcess, "wp", (char *)FOREGROUND, STD_IN, (char *)p[1]};
        createProcess(5, argv2);
        wait(wcId);
        close(pipeId);
        return 0;
    }
    else
        return createProcess(5, argv);
}

int filterVowels(char * str1, char * str2) {
    int i = 0, j = 0;
    while (str1[i] != 0){
        if (!isVowel(str1[i]))
            str2[j++] = str1[i];
        i++;
    }
    str2[j++] = 0;
    return j;
}

void filterFunction(){
    char filterBuff[100];
    char auxBuff[100];
    int size;
    do {
        read(2, filterBuff, 100);
        size = filterVowels (filterBuff, auxBuff);
        write(3, auxBuff, size);
    } while(filterBuff[0] != '$');
    exit();
}

int filter(uint64_t input, uint64_t output, int left){
    char * argv[] = {(char *)filterFunction, "filter", BACKGROUND, (char *)input,(char *) output};
    if (left == 1) {
        int p[2];
        int pipeId = pipe(p);
        argv[3] = (char*) p[0];
        int filterId = createProcess(5, argv);
        char * argv2[] = {(char *)writingProcess, "wp", (char *)FOREGROUND, STD_IN, (char *)p[1]};
        createProcess(5, argv2);
        wait(filterId);
        close(pipeId);
        return 0;
    }
    else
        return createProcess(5, argv);
}
void loop() {
    int pidPrueba = getPid();
    int i;
    while(1) {
        for (i=0; i < 100000000; i++);
        putDec(pidPrueba);
    }
}

void createLoop() {
    uint64_t fg = (shellBuffer[4] == '&') ? BACKGROUND : FOREGROUND;
    char * argv[] = {(char *)loop,"Loop",(char *) fg, STD_IN, (char *)STD_OUT};
    createProcess(5, argv);
}

void createPhylo() {
    uint64_t fg = (shellBuffer[5] == '&') ? BACKGROUND : FOREGROUND;
    char * argv[] = {(char *)mainPhylo, "PhyloProcess", (char *)fg, STD_IN, (char *)STD_OUT,(char *) 5};
    createProcess(6, argv);
}

int commandSwitch(int command,int input,int output, int left) { //juntar todos los procesos pipeables y poner el numero en process command
    switch(command) {
        case 0:help();break;
        case 1:time();break;
        case 2:cpuData();break;
        case 3:cpuTemp();break;
        case 4:printMem();break;
        case 5:inforeg();break;
        case 6:printf(listProcesses());break;
        case 7:nice();break;
        case 8:block();break;
        case 9:kill();break;
        case 10:createLoop();break;
        case 11:printf(listSemaphores());break;
        case 12:printf(listPipes());break;
        case 13:createPhylo();break;
        case 14:TMM();break;
        case 15:TPRO();break;
        case 16:TPRI();break;
        case 17:TSYNC();break;
        case 18:return cat(input, output, left);break;
        case 19:return wc(input, output, left);break;
        case 20:return filter(input, output, left);break;
        default:printf("Error: command doesnt exist\n");
    }
    return 0;
}

void processPipes(int izq,int der) {
    //izq entrada: standar salida:pipe
    //der entrada: pipe salida: standar
    int joinPipe[2];
    int joinPipeId = pipe(joinPipe);
    if (joinPipeId == -1)
        return;
    int derId = commandSwitch(der, joinPipe[0], STD_OUT, 0);
    commandSwitch(izq, STD_IN, joinPipe[1], 1);
    wait(derId);
    close(joinPipeId);
}

void processCommand() {
    putChar('\n');
    int command = matchArray();
    if (command == -1) {
        printf("Error: command doesnt exist\n");
        return;
    }
    int len = strlen(commandArray[command]);
    if (shellBuffer[len] == ' ' && shellBuffer[len + 1] == '|' && shellBuffer[len + 2] == ' ') {
        if (command < 18) {
            printf("Error: first command doesnt support pipes\n");
            return;
        }
        strcpy(shellBuffer,&shellBuffer[len + 3]);
        int command2 = matchArray();
        if (command2 < 18) {
            printf("Error: second command doesnt exist or doesnt support pipes\n");
            return;
        }
        processPipes(command, command2);
    }
    else
        commandSwitch(command, STD_IN, STD_OUT, 1);
}

void initShell() {
    while (1) { 
        putChar('>');
        for(int i=0; i < 100; i++)
            shellBuffer[i] = 0;
        char c;
        getChar(&c);
        while (c != '\n' ) {
            switch(c) {
                case BACKSPACE: shellBackSpace();break;
                case TAB: shellCE();break;
                default:if (shellPos < 100) {
                            putChar(c);
                            shellBuffer[shellPos++] = c;
                        }
            }
            getChar(&c);
        }
        shellBuffer[shellPos]=0;
        shellPos = 0;
        processCommand();
    }
}