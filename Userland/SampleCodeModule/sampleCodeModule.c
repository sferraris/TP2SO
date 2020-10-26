// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>
/* sampleCodeModule.c */

int main() {
    char * argv[] = {(char *)initShell, "Shell", (char *)1, 0,(char *) 1};
    createProcess(5, argv);
	return 0;
}