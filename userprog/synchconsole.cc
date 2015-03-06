#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"
#include "console.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *semaphorePutChar;
static Semaphore *semaphoreGetChar;
static Semaphore *semaphorePutString;
static Semaphore *semaphoreGetString;

static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console(readFile, writeFile, ReadAvail,WriteDone,0);

	semaphorePutChar = new Semaphore("PutChar",1);//initialiser les semaphores pour le synch
	semaphoreGetChar = new Semaphore("GetChar",1);
	semaphorePutString = new Semaphore("PutString",1);
	semaphoreGetString = new Semaphore("GetString",1);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
	
}

char SynchConsole::SynchGetChar()
{
// ...
}

void SynchConsole::SynchPutString(const char s[])
{
// ...
}

void SynchConsole::SynchGetString(char *s, int n)
{
// ...
}
#endif // CHANGED