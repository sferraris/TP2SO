#include <shell.h>

char shellBuffer[100] = {0};
char * commandArray[COMMANDS]= {"help", "time", "cpudata", "cputemp", "mem","inforeg", "zerotest", "opcodetest","ps","nice","block", "kill","loop", "sem", "pipe", "cat", "wc", "filter", "phylo", "testmm", "testprocesses", "testprio", "testsem"};
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
    printf("zerotest: Triggers exception 0\n");
    printf("opcodetest: Triggers exception 6\n");
    printf("ps: Lists all active processes\n");
    printf("nice: Changes priviledge level from a given process. Format: nice pri pid\n");
    printf("block: blocks a process given its pid\n");
    printf("loop: kills a process given its pid\n");
    printf("loop: Prints pid after a certain amount of seconds\n");
    printf("sem: Prints the name, status and ids of the blocked processes for each active semaphore\n");
    printf("pipe: Prints the id, status and ids of the blocked processes for each active pipe\n");
    printf("cat: Prints stdin as received\n");
    printf("wc: Counts the lines from the input\n");
    printf("filter: Filters the vowels from the input\n");
    printf("phylo: Simulates the phylosopher problem\n");
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
    char * argv[] = {testMM, "Testmm", 0, 0, 1};
    createProcess(5, argv);
}

void TPRO(){
    char * argv[] = {proTest, "testprocesses", 0, 0, 1};
    createProcess(5, argv);
}

void TPRI(){
    char * argv[] = {priorityTest, "testprio", 0, 0, 1};
    createProcess(5, argv);
}

void TSYNC(){
    char * argv[] = {Tsync, "testsem", 0, 0, 1};
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
                        shellBuffer[0] = 1;
                        shellBuffer[1] = 0;
                        write(3, shellBuffer, 1);
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
    } while(catBuf[size-1] != 1);
    write(3, "\n", 1);
    exit();
}

int cat(int input, int output, int left){
    char * argv[] = {catFunction, "cat", 0, input, output};
    if (left == 1) {
        int p[2];
        int pipeId = pipe(p);
        argv[3] = p[0];
        int catID = createProcess(5, argv);
        char * argv2[] = {writingProcess, "wp", 1, 0, p[1]};
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
    } while(wcBuff[0] != 1);
    write(3,"Line count: ", 13);
    putDec(lineCount);
    write(3, "\n", 1);
    wcBuff[0] = 1;
    wcBuff[1] = 0;
    write(3, wcBuff, 1);
    exit();
}

int wc(int input, int output, int left){
    char * argv[] = {wcFunction, "wc", 0, input, output};
    if (left == 1) {
        int p[2];
        int pipeId = pipe(p);
        argv[3] = p[0];
        int wcId = createProcess(5, argv);
        char * argv2[] = {writingProcess, "wp", 1, 0, p[1]};
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
    } while(filterBuff[size-1] != 1);
    write(3, "\n", 1);
    exit();
}

int filter(int input, int output, int left){
    char * argv[] = {filterFunction, "filter", 0, input, output};
    if (left == 1) {
        int p[2];
        int pipeId = pipe(p);
        argv[3] = p[0];
        int filterId = createProcess(5, argv);
        char * argv2[] = {writingProcess, "wp", 1, 0, p[1]};
        createProcess(5, argv2);
        wait(filterId);
        close(pipeId);
        return 0;
    }
    else
        return createProcess(5, argv);
}

int commandSwitch(int command,int input,int output, int left) { //juntar todos los procesos pipeables y poner el numero en process command
    switch(command) {
        case 0:help();break;
        case 1:time();break;
        case 2:cpuData();break;
        case 3:cpuTemp();break;
        case 4:printMem();break;
        case 5:inforeg();break;
        case 6:zero_test();break;
        case 7:invalidopcode_test();break;
        case 8:printf(listProcesses());break;
        case 9:nice();break;
        case 10:block();break;
        case 11:kill();break;
        case 12:createLoop();break;
        case 13:printf(listSemaphores());break;
        case 14:printf(listPipes());break;
        case 15:return cat(input, output, left);break;
        case 16:return wc(input, output, left);break;
        case 17:return filter(input, output, left);break;
        case 18:createPhylo();break;
        case 19:TMM();break;
        case 20:TPRO();break;
        case 21:TPRI();break;
        case 22:TSYNC();break;
        default:printf("Error: command doesnt exist\n");
    }
}

void processPipes(int izq,int der) {
    //izq entrada: standar salida:pipe
    //der entrada: pipe salida: standar
    int joinPipe[2];
    int joinPipeId = pipe(joinPipe);
    if (joinPipeId == -1)
        return;
    int derId = commandSwitch(der, joinPipe[0], 1, 0);
    commandSwitch(izq, 0, joinPipe[1], 1);
    wait(derId);
    close(joinPipeId);
}

void processCommand() {
    putChar('\n');
    int command = matchArray();
    int len = strlen(commandArray[command]);
    if (shellBuffer[len] == ' ' && shellBuffer[len + 1] == '|' && shellBuffer[len + 2] == ' ') {
        if (command == -1) {
            printf("Error: first command doesnt support pipes\n");
            return;
        }
        strcpy(shellBuffer,&shellBuffer[len + 3]);
        int command2 = matchArray();
        if (command2 == -1) {
            printf("Error: second command doesnt support pipes\n");
            return;
        }
        processPipes(command, command2);
    }
    else
        commandSwitch(command, 0, 1, 1);
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
    int fg = (shellBuffer[4] == '&') ? 0 : 1;
    char * argv[] = {loop, "Loop", fg, 0, 1};
    createProcess(5, argv);
}

void initShell() {
    while (1) { 
        putChar('>');
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