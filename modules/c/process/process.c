#include <std/std.h>
#include <process/process.h>
#include <video/video.h>

static Process* first;

static Process* current;

extern void _process_init();
extern void callProcess(void* stack);
extern void retFromProcess();

static int getNextPid();

/* user-mode exit routine */
static void proc_exit();


void process_init() {
	first = NULL;
	current = NULL;
	_process_init();
}

static Process* newProcess(char* name, void* NPC, int stackSize) {
	Process* new = (Process*)allocmem(sizeof(Process));
	if(new == 0) {
		printString("OUT OF CHEESE ERROR - PLEASE REINSTALL UNIVERSE\n");
		while(1);
	}
	int namesize = strlen(name)+1;
	new->name = (char*)allocmem(namesize);
	strcpy(new->name,name);

	new->stack = (char*)allocmem(stackSize);	/* allocate stack */

	/* Need to create initial stack frame:
		[null saved ebp]
		[proc_exit address]
		[saved ebp(address of the null ebp above)]  <-- initial process frame pointer and stack pointer
		[process start address]
		[Registers in initial states]
	*/

	int sp = (int)new->stack + stackSize-1; /* point to last byte in stack space */

	sp-=4;				/* push null saved ebp(never used) */
	*((void**)sp) = 0;
	int savedFp = sp;

	sp-=4;
	*((void**)sp) = proc_exit;	/* store return address for exit code */

	int savedSp = sp;	/* this is the process' initial stack/frame pointer */

	/* end of user-stack frame setup */
	

	sp-=4;
	*((void**)sp) = NPC;	/* process start address */

	/* Initial register state push: */
	sp-=32;		/* Subtract from stack pointer 32 bytes to make space for register states */
	int* savedRegs = (int*)sp;
	savedRegs[3] = savedFp;		/* save initial esp and ebp */
	savedRegs[2] = savedSp;


	/* save stack pointer in bottom of process' stack space */
	((int*)new->stack)[0] = sp;

	/* set delproc flag to false */
	new->delproc = 0;

	/* return pointer to new process struct */
	return new;
}

static int getNextPid() {
	Process* p = first;
	int i = p->pid;
	while(p->pid == (i++)) p = p->next;
	return i;
}

static void freeProcess(Process* p) {
	freemem(p->stack);
	freemem(p->name);
	freemem(p);
}

int addProcess(char* pname, void* NPC, int stackSize) {
	Process** p = &first;
	while(*p) p = &((*p)->next);
	
	Process* new = newProcess(pname,NPC,stackSize);
	*p = new;
	new->next = NULL;
	new->pid = getNextPid();

	if(!current) current = new;

	return new->pid;
}


void rmProcess(int pid) {
	Process** i = &first;

	while(*i && (*i)->pid != pid) i = &((*i)->next);

	if(!*i) return;

	Process* del = *i;

	*i = del->next;

	freeProcess(del);
}

int nextProcess() {
	if(current->next)
		current = current->next;
	else
		current = first;

	return current->pid;
}

int do_work() {
	/* call to context-switching code, returns when code gives up CPU, or is preempted */
	callProcess(current->stack);

	if(current->delproc) {			/* if process has exited */
		int delpid = current->pid;
		nextProcess();
		rmProcess(delpid);
		printString("\nProcess deleted\n");
		return 1;
	}
	return 0;
}


static void printProcessState() {
	/* display details of current process state while sleeping */
	#define printa(a,b) printString(a); printString(": "); printHexInt((unsigned int)b); printChar('\n');	

	int* stk = ((int**)(current->stack))[0];

	printa("Stack segment start",current->stack);
	printa("State storage base",stk);
	printa("PID",current->pid);
	printa("Delete flag",current->delproc);
	printa("NPC",stk[8]);
	printa("EAX",stk[7]);
	printa("ECX",stk[6]);
	printa("EDX",stk[5]);
	printa("EBX",stk[4]);
	printa("ESP",stk[3]);
	printa("EBP",stk[2]);
	printa("ESI",stk[1]);
	printa("EDI",stk[0]);

}

int getCurrentPid() {
	if(!current) {
		return 0;
	}
	return current->pid;
}

char* getCurrentProcName() {
	if(!current) {
		return "Pre-init";
	}
	return current->name;
}




/* USER MODE ROUTINE, never returns */
void proc_exit() {		/* This code is called when a process' stack frame returns, call address is embedded in frame #0 */
	current->delproc = 1;	/* set delproc flag to true */
	printString("\nMarking process for deletion\n");	/* notify user */
	retFromProcess();	/* jump back to kernel space where this process will be deleted */
}
