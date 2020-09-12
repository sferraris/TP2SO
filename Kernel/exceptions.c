#include <exceptions.h>

static void zero_division();
static void invalid_opcode();

char* regNames[17] = {"RIP: ", "RSP: ", "RAX: ", "RBX: ","RCX: ","RDX: ","RBP: ","RDI: ","RSI: ","R8: ","R9: ","R10: ","R11: ","R12: ","R13: ","R14: ","R15: ",};

void exceptionDispatcher(int exception)
{
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	if ( exception == INVALID_OPCODE_EXCEPTION_ID)
		invalid_opcode();
}

void throwException(char* buff)
{
	int aux[17];
	recoverExceptionRegisters(aux);
	printStringRed(buff);
	for ( int i = 0; i < 17; i++)
	{
		printStringRed(regNames[i]);printHex(aux[i]);printChar('\n');
	}
	printString("Rebooting...\nBooted succesfully\n");
	updateIp();
}

static void zero_division() 
{
	throwException("Exception 0: Cant divide by 0\n");
}

static void invalid_opcode() 
{
	throwException("Exception 6: Invalid Operand Code\n");
}