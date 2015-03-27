#ifdef CHANGED
#include "userthread.h"
#include "thread.h"
#include "system.h"
#include "addrspace.h"

static void StartUserThread(int f)
{
	argThreadUser *argRecup = (argThreadUser *) f; //Recuperation des parametres passes precedements

	//initialisation des registres
	//Mise en place le PC correctement, en debut de fonction
	machine->WriteRegister(PCReg,argRecup->f);
	//Mise en place ensuite l'argument dans le registre 4
	machine->WriteRegister(4, argRecup->arg);
	//Mise en place du PC Next pour l'instruction suivante
	machine->WriteRegister(NextPCReg, argRecup->f+4);
	//Initialisation de bitmap
	machine->WriteRegister(StackReg, currentThread->space->InitRegistersU(&(currentThread->id))); 

	//LANCER
	currentThread->space->addThread();
	machine->Run();
}

int do_UserThreadCreate(int f, int arg)
{
	Thread *newThread = new Thread("new thread");
	//check if the newly created thread is null
	if(newThread == NULL)
	{
		return -1;
	}

	//arguments concatenate
	argThreadUser *argv = new argThreadUser;
	argv->f = f;
	argv->arg = arg;

	newThread->id = currentThread->space->getID();
	if(newThread->id == -1)
	{
		//erreur 
		printf("\nError during the creation of a new thread\n");
		delete newThread;
		return -1;
	}else{
		//place into the thread queue
		newThread->Fork(StartUserThread, (int)argv);
	}

	currentThread->space->semJoin[newThread->id]->P();

	machine->WriteRegister(2,newThread->id); //get the thread id
	currentThread->Yield();    // Sans cela, obligation d'effectuer une action  dans le main, car les threads sont justes créés mais
    						// pas encore démarrés
	return 0;
}

void do_UserThreadExit()
{
	currentThread->space->semJoin[currentThread->id]->V(); //release one resource of user threads holding pool
	currentThread->space->deleteThread(); //remove the resource of this user thread from memory
	currentThread->Finish(); //
}

void do_UserThreadJoin(int i)
{
	currentThread->space->semJoin[i]->P();
}

#endif //CHANGED