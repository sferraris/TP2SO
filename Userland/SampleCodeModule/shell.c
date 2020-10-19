#include <shell.h>

char shellBuffer[100] = {0};
char * commandArray[COMMANDS]= {"help", "time", "cpudata", "cputemp", "mem","inforeg", "zerotest", "opcodetest","ps","nice","block", "kill","loop", "sem", "pipe", "cat", "wc", "filter", "phylo"};
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

void catFunction(int piped){
    char catBuff[250] = {0};
    int catIndex;
    while (1) { 
        char c;
        getChar(&c);
        while (c != '\n' ) {
            if ( catIndex < 250) { 
                putChar(c); //putCHAr a fd[0] - 0
                catBuff[catIndex++] = c;
            }
            if ( c == '$')
                exit();
            getChar(&c);
        }
        if ( c == '\n')
            putChar(c);
        catBuff[catIndex] = 0;

        catIndex = 0;
        if ( piped == 0 ){
            //printf(catBuff); //printf fd[2] - 2
            //putChar('\n');
        }
    }

}
void cat(int input, int output, int foreground, int piped){
    char * argv[] = {catFunction, "cat", foreground, input, output, piped};
    createProcess(6, argv);
}

void wcFunction(int piped){
    char wcBuff[250];
    int wcIndex;
    int lineCount = 0;
    char c;
    getChar(&c);
    while (c != '$' ) {
        if ( wcIndex < 250) { 
            if ( c == '\n' ){
                lineCount++;
                wcIndex = 0;
            }
            else
                wcBuff[wcIndex++] = c;
                putChar(c);
            }
        getChar(&c);
    }
    if ( wcIndex > 0){
        putChar('\n');
        lineCount++;
    }
    printf("Line count: ");
    putDec(lineCount);
    putChar('\n');
    exit();
}

void wc(int input, int output, int foreground, int piped){
    char * argv[] = {wcFunction, "wc", foreground, input, output, piped};
    createProcess(6, argv);
}

char * filterVowels(char * buff){
    char * aux = malloc(250 * sizeof(char));
    int i = 0, j = 0;
    while ( buff[i] != 0){
        if ( !isVowel(buff[i]) ){
            aux[j++] = buff[i];
            
        }
        i++;
    }
    aux[j] = 0;
    return aux;
}

void filterFunction(int piped){
    char filterBuff[250] = {0};
    int filterIndex = 0;
    while (1) { 
        char c;
        getChar(&c);
        filterBuff[filterIndex++] = c;
        while (c != '\n' ) {
            if ( filterIndex < 250) { 
                putChar(c);
                filterBuff[filterIndex] = c;
                filterIndex++;
            }
            if ( c == '$')
                exit();
            getChar(&c);
        }
        if ( c == '\n')
            putChar(c);
        
        filterBuff[filterIndex] = 0;
        filterIndex = 0;
    
        char * filtered = filterVowels(&filterBuff[1]);
    
        printf(filtered);
        free(filtered);
        
        putChar('\n');
    }
}

void filter(int input, int output, int foreground, int piped){
    char * argv[] = {filterFunction, "filter", foreground, input, output, piped};
    createProcess(6, argv);
}

void commandSwitch(int command,int input,int output,int foreground, int piped) {
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
        case 15:cat(input, output,foreground, piped);break;
        case 16:wc(input, output,foreground, piped);break;
        case 17:filter(input, output,foreground, piped);break;
        case 18:createPhylo();break;
        default:printf("Error: command doesnt match\n"); 
    }

}

void processPipes(int izq,int der) {
    //izq eentrada: standar salida:pipe
    //der entrada: pipe salida: standar
    int processPipe[2];
    int index = pipe(processPipe);
    if (index == -1)
        return;
    commandSwitch(der,processPipe[1],1,0, 1);
    //aca
    commandSwitch(izq,0,processPipe[0],1, 1);
    exit();
}

void processCommand() {
    putChar('\n');
    int command = matchArray();
    int len = strlen(commandArray[command]);
    if (shellBuffer[len] == ' ' && shellBuffer[len + 1] == '|' && shellBuffer[len + 2] == ' ') {
        strcpy(shellBuffer,&shellBuffer[len + 3]);
        int command2 = matchArray();
        char * argv[] = {processPipes, "processPipes", 0, 0, 1, command, command2};
        createProcess(7, argv);
    }
    else {
        commandSwitch(command,0,1,1, 0);
    }
    
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

void readPipe() {
    char buf[50];
    while (1) {
        read(buf, 50);
        printf(buf);
        if (strcmp(buf, "Exit"))
            exit();
    }
}

void writePipe() {
    for(int i=0; i < 11; i++) {
        for (int j=0; j < 100000000; j++);
        printf("Hola Mundo\n");
    }
    printf("Exit");
    exit();
}

void pipeTest() {
    int p[2];
    int index = pipe(p);
    char * argv1[] = {readPipe, "readPipe", 0, p[0], 1};
    createProcess(5, argv1);
    char * argv2[] = {writePipe, "writePipe", 0, 0, p[1]};
    createProcess(5, argv2);
    //close(index);
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
                case '5': mainPhylo(5);break;
                default:if ( shellPos < 100) { 
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