
#include "syscall.h"

int main()
{
	char s = SynchGetChar();
	SynchPutChar(s);
	SynchPutChar('\n');
	return 0;
}