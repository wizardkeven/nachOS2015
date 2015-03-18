// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"



//-----------------------------------------------------------------------
// copyStringFromMachine : use for PutString
//-----------------------------------------------------------------------

#ifdef CHANGED
void copyStringFromMachine(int from, char *to, unsigned size)
{
  unsigned i = 0;
  int tmp;
  for(i = 0; i < size ; i++){
    if(machine->ReadMem(from + i, 1, &tmp))
    to[i]=tmp;
  }
  //si le message ne se fini pas par '\0'...
  if(i<size && tmp != '\0'){
    to[size-1] = '\0';
  }
}
#endif // CHANGED


//-----------------------------------------------------------------------
// copyStringToMachine : use for GetString, fonction pratiquement 
// identique
//-----------------------------------------------------------------------

#ifdef CHANGED
void copyStringToMachine(char *from, int to, unsigned int size)
{
  int tmp;
  unsigned int i;
  for(i = 0; i < size - 1; i++){
    tmp = from[i];
    machine->WriteMem(to + i, 1, tmp);
  }
  tmp = '\0';
  machine->WriteMem(to + i, 1, tmp);
}
#endif // CHANGED


//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);
    #ifndef CHANGED // Noter le if*n*def
    if ((which == SyscallException) && (type == SC_Halt)) {
    DEBUG('a', "Shutdown, initiated by user program.\n");// Obligatoire pour ne pas avoir de bug à l'utilisation de filesys
      interrupt->Halt();
    } else {
      printf("Unexpected user mode exception %d %d\n", which, type);
      ASSERT(FALSE);
    }
    UpdatePC();//
    #else // CHANGED

    if (which == SyscallException)
      {
        switch(type){

          case SC_Halt:{
            DEBUG ('a', "Shutdown, initiated by user program.\n");
            interrupt->Halt ();
            break;
          }
          case SC_Exit:{
            DEBUG('a', "Exit by user thread.");
            interrupt->Halt();
            break;
          }
          case SC_SynchPutChar:{
            int lecture = machine->ReadRegister(4);//Lire le registre 4 qui contient l'argument de la
                                                  // fonction appele
            synchconsole->SynchPutChar((char) lecture); //Afficher cet argement

            break;
          }
          case SC_SynchPutString: {
            char *buffer = new char[MAX_STRING_SIZE]; 
            int recup = machine->ReadRegister(4);
            copyStringFromMachine(recup, buffer, MAX_STRING_SIZE);
            // printf("SynchPutString %s\n", buffer);
            synchconsole->SynchPutString(buffer);
            delete [] buffer;
            break;
          }
          case SC_SynchGetChar: {
            char recup = synchconsole->SynchGetChar();
            machine->WriteRegister(2, (int)recup);
            break;
          }
          case SC_SynchGetString: {
            char *buffer = new char[MAX_STRING_SIZE];
            int recup = machine->ReadRegister(4);
            int taille = machine->ReadRegister(5);
            synchconsole->SynchGetString(buffer, taille);
            copyStringToMachine(buffer, recup, taille);
            delete buffer;
            break;
          }
          case SC_SynchPutInt: {
            int recup = machine->ReadRegister(4);
            synchconsole->SynchPutInt(recup);
            break;
          }
          case SC_SynchGetInt: {
            int *recup = new int;
            *recup = machine->ReadRegister(4);
            synchconsole->SynchGetInt(recup);
            delete recup;
            break;
          }
          default:{
            printf("Unexpected user mode exception %d %d\n", which, type);
            ASSERT(FALSE);
          }
        }
      //   // LB: Do not forget to increment the pc before returning!
      //   UpdatePC();
      // // End of addition
      }
   //  else
   //    {
	  // printf ("Unexpected user mode exception %d %d\n", which, type);
	  // ASSERT (FALSE);
   //    }

    // LB: Do not forget to increment the pc before returning!
    UpdatePC();
    // End of addition
      #endif // CHANGED
}
