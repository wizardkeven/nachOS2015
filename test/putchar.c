#ifdef CHANGED
#include "syscall.h"

void print(char c, int n)
{
	int i;
	for(i =0;i<n;i++){
		// PutChar(c+i);
		int ss=0;
		ss *=i;
	}
	// PutChar('\n');
}

int main()
{
	print('a',4); 
	Halt();
}
#endif //CHANGED