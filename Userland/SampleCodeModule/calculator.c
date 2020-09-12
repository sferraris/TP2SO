#include <calculator.h>
char calcBuffer[100] = {0};
int calcPos = 0;
int calcInitiateProcess = 0;
void processCalculation ();
double calculate(char *buffer, int *pos, int *flag);

int isNumber(char c) 
{
	if (c >= '0' && c <= '9')
		return 1;
	return 0;
}

int isOperator(char c) 
{
	if (c == '+' || c == '-' || c == '%' || c == 'x')
		return 1;
	return 0;
}

void clearNum(int num[],int j) 
{
	for (int k=0; k < j; k++) 
		num[k] = 0;
}

double pow(double num,int potencia) 
{
	double rta;
	if (num == 0)
		return 0;
	else if (potencia == 0)
		return 1;
	else if (potencia >= 0) 
	{
		rta = num;
		for (int i=1; i<potencia;i++) 
			rta *= num;
	}
	else 
	{
		rta=1.0;
		potencia = potencia * (-1);
		for (int i=1; i<=potencia;i++) 
			rta /=  num;
	}
	return rta;
}

void calcBackSpace()
{
    if (calcPos)
    {
        calcBuffer[calcPos] = 0;
        calcPos--;
        putChar(BACKSPACE);
    }
}

void calcCE()
{
	while(calcPos)
		calcBackSpace();
}

void initCalc()
{
	if ( !calcInitiateProcess)
		sendIp();
	if ( !calcInitiateProcess)
	{
		calcInitiateProcess = 1;
		return;
	}
	 while (1) 
    { 
        char c = getChar();
        while (c != '=' )
        {
            switch(c)
            {
                case BACKSPACE: calcBackSpace();break;
				case TAB: calcCE();break;
                case ESC: switchWindow();break;
                default:if ( calcPos < 100)
						{ 
							if(isNumber(c) || isOperator(c) || c == '(' || c == ')' || c == '.')
						 	{ 
						 		putChar(c);
						 		calcBuffer[calcPos++] = c;
						 	}
						}
            }
            c = getChar();
        }
		if (calcBuffer[0] != '(') 
			printf("\nOperation must start with (\n");
		else
		{
			calcBuffer[calcPos] = 0;
			processCalculation();
		}
		calcPos = 0;
    }
}

double createNumber(char *buffer,int *pos,int *flag) 
{
	double res = 0;
	int sign = ((buffer[*pos] == '-')? (-1) : 1);
	if (sign == -1) 
		*pos += 1;
	while (buffer[*pos] != '.' && !isOperator(buffer[*pos]) && buffer[*pos] != ')') 
	{
		if (!isNumber(buffer[*pos])) 
		{
			*flag = 1;
			return 0;
		}
		res = res * 10.0 + (double) (buffer[*pos] - '0');
		*pos += 1;
	}
	if (buffer[*pos] == '.') 
	{
		*pos += 1;
		int i = -1;
		while (!isOperator(buffer[*pos]) && buffer[*pos] != ')') 
		{
			if (!isNumber(buffer[*pos])) 
			{
				*flag = 1;
				return 0;
			}
			res += (double) (buffer[*pos] - '0') * pow(10.0,i);
			*pos += 1;
			i--;
		}
	}
	return sign * res;
}

double zero_division_validator(double num1, double num2);

double operate(double num1, double num2, char operator)
{
	switch(operator)
	{
		case '+': return num1+num2;break;
		case '-': return num1-num2;break;
		case 'x':return num1*num2;break;
		case '%': return zero_division_validator(num1, num2);
	}
	return 0;
}

double zero_division_validator(double num1, double num2)
{
	if ( num2 == 0.0)
	{
		printf("Cant divide by"); 
		return 0.0;
	}
	return num1/num2;
}

void processCalculation ()
{
	putChar('\n');
	int flag = 0;
	int pos = 0;
	double resp = calculate(calcBuffer, &pos, &flag);
	if (flag == 0)
		putDouble(resp);
	else
		printf("ERROR: malformed operation");
	putChar('\n');
} 

double calculate(char *buffer, int *pos, int *flag)
{
	if ( *flag == 1)
		return -1;
	if (buffer[*pos] == '-' || isNumber(buffer[*pos])) 
	{
		double aux = createNumber(buffer,pos,flag);
		return aux;
	}
	else if ( buffer[*pos] == '(')
	{
		*pos += 1;
		double left = calculate(buffer, pos, flag);
		if ( !isOperator(buffer[*pos]))
		{
			*flag = 1;
			return -1;
		}
		char operator = buffer[*pos];
		*pos += 1;
		double right = calculate(buffer, pos, flag);
		if ( buffer[*pos] != ')')
		{
			*flag = 1;
			return -1;
		}
		*pos += 1;
		return operate(left, right, operator);
	}
	else
	{
		*flag = 1;
		return -1;
	}
}