#include <sysinterface/sysinterface.h>
#include <std/std.h>
#include <mm/mm.h>


static Interface* first;

static Interface* newInterface(char* name,int (*func)()) {
	Interface* new = (Interface*)allocmem(sizeof(Interface));
	new->func = func;
	int namesize = strlen(name)+1;
	new->name = (char*)allocmem(namesize);
	strcpy(new->name,name);
	return new;
}

static freeInterface(Interface* del) {
	freemem(del->name);
	freemem(del);
}

void sysinterface_init() {
	first = NULL;
}


Interface* addInterface(char* ifname, int (*func)()) {
	Interface* newif = newInterface(ifname,func);

	//get ptr to last interface pointer
	Interface** i = &first;
	while(*i) i = &((*i)->next);

	*i = newif;
	newif->next = NULL;

	return newif;
}

void rmInterface(int (*funcptr)()) {
	Interface** i = &first;

	//iterate through list until i points to address of struct to delete
	while(*i && (*i)->func != funcptr) i = &((*i)->next);

	if(!*i) return;	//if struct not found, fail quietly

	Interface* del = *i;

	//take struct to delete out of list
	*i = del->next;
	
	freeInterface(del);	//deallocate memory
}

int (*getInterface(char* ifname))() {
	Interface* i = first;
	while(i && strcmp(i->name,ifname)) i = i->next;
	if(!i) return NULL;
	return i->func;
}
