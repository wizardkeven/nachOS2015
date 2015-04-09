#include "syscall.h"

void print(int num)
{
	int j=0;
	for(j=0;j<3;j++)
	{
		if(num%2 == 0)
		{
			SynchPutString("Un thread de parametre paire est en execution!");
		}else
		{
			SynchPutString("Un thread de parametre impaire est en execution!");

		}
	}
	UserThreadExit();
}

int main()
{

	int t1, t2;
	int t3,t4,t5,t6;

	t1 = UserThreadCreate(print,(void *) 7);
	t2 = UserThreadCreate(print,(void *) 8);
	t3 = UserThreadCreate(print,(void *) 7);
	t4 = UserThreadCreate(print,(void *) 8);
	t5 = UserThreadCreate(print,(void *) 7);
	t6 = UserThreadCreate(print,(void *) 8);
	SynchPutString("Je suis le thread principal");
	


	return 0;
}