#include <shell.h>
/* sampleCodeModule.c */

int main() {
    char * argv[] = {initShell, "Shell", 1};
    createProcess(3, argv);
	//no imprime
	return 0;
}