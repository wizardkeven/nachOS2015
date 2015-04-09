#include "syscall.h"


void print(int i){
	int j = 0;
	for(j = 0; j < 5; j++){
		SynchPutChar(i);	
		SynchPutChar('\n');	
	}

	UserThreadExit();
}

int main(){
	int t1, t2;
	t1 = UserThreadCreate(print,(void *) 'a');
	t2 = UserThreadCreate(print,(void *) 'n');

	UserThreadJoin(t1);
	UserThreadJoin(t2);


	return 0;
}