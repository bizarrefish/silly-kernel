/* process module header */

struct p_type {
	struct p_type* next;
	int pid;
	int delproc;	/* flag to mark process for deletion */
	char* stack;	/* stack segment, first 4 bytes is saved stack pointer */
	char* name;	/* process name */
};

typedef struct p_type Process;

void process_init();

/* makes new process, returns pid */
int addProcess(char* pname, void* NPC, int stackSize);

/* removes process from process queue(kill) */
void rmProcess(int pid);

/* advances process in queue, returns new process' pid */
int nextProcess();

/* does processing for current process, returns 0 if process died */
int do_work();

/* Gets current process' pid */
int getCurrentPid();

/* get current process' name */
char* getCurrentProcName();
