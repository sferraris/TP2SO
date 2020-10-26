#include <shell.h>
/* sampleCodeModule.c */

int main() {
    char * argv[] = {(char *)initShell, "Shell", (char *)1, 0,(char *) 1};
    createProcess(5, argv);
	return 0;
}