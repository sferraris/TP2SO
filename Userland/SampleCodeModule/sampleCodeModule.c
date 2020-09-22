#include <shell.h>
/* sampleCodeModule.c */

int main() {
	printf("Inicio de shell");
	putHex(&initShell);
	createProcess(&initShell);
	return 0;
}