#include "syscall.h"

int main()
{
	// 
	SynchPutString("What does 4*5 equal?");
	int i;
	SynchGetInt(&i);

	if(i != 20)
	{
		SynchPutString("False");
	}else
	{
		SynchPutInt(i);
	}
	SynchPutString("What much do you spend for your computer?");
	SynchGetInt(&i);
	SynchPutString("You spent: ");
	SynchPutInt(i);
	SynchPutChar('\n');
	return 0;
} 