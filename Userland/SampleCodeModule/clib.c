#include <clib.h>
char operationBuffer[100] = {'0'};

void putChar(char c) {
    write(1,c);
}

void printf(char * buffer) {
    write(2, buffer);
}

void printRed(char * buffer) {
    write(3, buffer);
}

char getChar() {
    return read(); 
}

void getmem(int num, uint64_t*buffer) {
    readmem(num, buffer);
}

void putDouble(double num) {
    if (num < 0) {
        num *= -1.0;
        putChar('-');
    }
    putDec((int) num);
    putChar('.');
    num = (num - (int) num) * 10000;
    int aux = 1000;
    while (num < aux) {
        putDec(0);
        aux = aux / 10;
    }
    if ( (int) num != 0)
        putDec((int) num );
}

int strcmpParams(char * str1, char * str2) {
    if ( str1[0] == 0)
        return 0;
    int i;
    for (i=0; str1[i] && str2[i]; i++) {
        if (str1[i]!=str2[i])
            return 0;
    }
    return (str1[i] == ' ' || str1[i] == 0 || str1[i] == '&') && str2[i] == 0;
}

int strcmp(char * str1, char * str2) {
    if ( str1[0] == 0)
        return 0;
    int i;
    for (i=0; str1[i] && str2[i]; i++) {
        if (str1[i]!=str2[i])
            return 0;
    }
    return str1[i] == 0  && str2[i] == 0;
}

int getHours() {
   return getTime(4);
}

int getMinutes() {
    return getTime(2);
}

int getSeconds() {
    return getTime(0);
}

char * getCpuVendor(char * buffer) {
    return getCpuData(1, buffer);
}

char * getCpuBrand(char * buffer) {
    return getCpuData(2, buffer);
}

int getCpuModel() {
    return readCpuModel();
}

int* receiveRegisters() {
    return readRegisters();
}

int isNumberHexa(char c) {
    return c >= '0' && c <= '9';
}

int isLetterHexa(char c) {
    return (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

int stringHexToInt(char * s) {
    int i=0;
    int num;
    int hex = 0;
    while (s[i] != 0) {
        if (isNumberHexa(s[i])) 
            num = s[i] - '0'; 
        else if (isLetterHexa(s[i])) {
            if (s[i] >= 'a')
                num = (s[i] - 'a') + 10;
            else 
                num = (s[i] - 'A') + 10;
        }
        hex = hex * 16 + num;
        i++;
    }
    return hex;
}

uint32_t uintToBase(uint64_t value, uint32_t base) {
	char *p = operationBuffer;
	char *p1, *p2;
    uint32_t digits = 0;
	//Calculate characters for each digit
	do {
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
	}
	while (value /= base);
	// Terminate string in buffer.
	*p = 0;
	//Reverse string in buffer.
	p1 = operationBuffer;
	p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
    return digits;
}

void putDec(uint64_t num) {
    uintToBase(num, 10);
    printf(operationBuffer);
}

void putHex(uint64_t num) {
    uintToBase(num, 16);
    printf(operationBuffer);
}

void putFilledHex (uint64_t num) {
    uint32_t digits = uintToBase(num, 16);
    for ( int i = 0; i < 16-digits; i++)
        putChar('0');
    printf(operationBuffer);
}

void putBin(uint64_t num) {
    uintToBase(num, 2);
    printf(operationBuffer);
}

uint64_t getTemp() {
    return getCpuTemp();
}

void * malloc(uint64_t size) {
    return getMalloc(size);
}

void free(void * p) {
    getFree(p);
}

int createProcess(int argc, char * argv[]){
    return createProcessAsm(argc, argv);
}

void exit(){
    exitAsm();
}

int getPid(){
    return getPidAsm();
}

int killProcess(int pid){
   return changeProcessStateAsm(pid,KILLED);
}

int changeProcessState(int pid,int state){
    return changeProcessStateAsm(pid,state);
}

int blockProcess(int pid) {
    changeProcessStateAsm(pid,BLOCKED);
}

char * listProcesses() {
    return listProcessesAsm();
}

void changePriority(int pid,int pri) {
    if (pri < 0 || pri > 9)
        printf("Numero incorrecto de privilegio\n");
    niceAsm(pid,pri);
}