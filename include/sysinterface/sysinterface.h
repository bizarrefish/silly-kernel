/* sysinterface header */


struct if_type {
	struct if_type* next;
	int (*func)();
	char* name;
};

typedef struct if_type Interface;

/* initialise interface system */
void sysinterface_init();

/* add interface, returning interface ptr */
Interface* addInterface(char* ifname, int (*func)());

/* remove interface */
void rmInterface(int (*funcptr)());

/* find interface by name */
/* returns interface function, or, on error, returns null */
int (*getInterface(char* ifname))();
