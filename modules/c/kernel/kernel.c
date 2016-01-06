#include <keyboard/keyboard.h>
#include <sysinterface/sysinterface.h>
#include <video/video.h>
#include <mm/mm.h>
#include <std/std.h>
#include <process/process.h>

extern void* _heapStart;
extern void* _heapEnd;
extern chunk* firstchunk;

void printMM() {
	printString("Chunk List:\n");
	chunk* c = firstchunk;
	while(c) {
		printString("Chunk at: "); printHexInt((unsigned int)c); printString("  Taken-"); printByte(c->taken); printChar('\n');
		c=c->next;
	}
}

void someCode();
void aProcess();

int kMain() {
	int heapSize = &_heapEnd - &_heapStart;
	mm_init(&_heapStart,heapSize);
	sysinterface_init();
	process_init();
	printString("Alloc 800KB\nNew Memory map:\n");
	allocmem(800*1024);
	printMM();
	printString("Create 2 interfaces\n");

	addInterface("Kernel Main",kMain);
	addInterface("printMM",(int(*)())printMM);

	printString("Get printMM Function: ");
	printHexInt((unsigned int)getInterface("printMM"));
	printChar('\n');

	printString("Starting Processes\n");
	addProcess("Proc",(void*)someCode,400);
	addProcess("Proc",(void*)aProcess,400);

	/* Main process loop */

	/* THIS IS DISGRACEFUL! Two context switches plus some condition code and other crap. Completely unnecessary! slow, too */
	while(1) {
		do_work();
		nextProcess();
	}
}

void aProcess() {
	printString("Process started\n");
	int i = 0;
	int j = 8000000;
	while(1) {
		//while(--j);
		j = 8000000;
		printString("A number: "); printByte(i); printChar('\n');
		i++;
		if(i == 11) {
			i = 0;
			printString("Spawning new process\n");
			addProcess("ChildProcess",(void*)aProcess,400);
		}
		printString("Giving up cpu\n");
		retFromProcess();
		allocmem(3000);
	}

}

void someCode() {
	retFromProcess();
	printString("Some text here\n");
	retFromProcess();
	printString("A little arithmetic\n");
	int i = 4+9;
	retFromProcess();
	printString("Byebye\n");
}
