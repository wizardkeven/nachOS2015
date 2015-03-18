#ifdef CHANGED
#include "syscall.h"

int main()
{
	char c[512];
	SynchGetString(c,128);
	// SynchPutString("after get string\n");
	SynchPutString(c);
	return 0;
}
#endif //CHANGED