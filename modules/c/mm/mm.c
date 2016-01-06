#include <mm/mm.h>

//platform-specific type to do pointer arithmetic(amd64 - long, x86 - int)
#define APType int

static void* memory;
#define null (void*)0;
#define PTR(a) (APType)((void*)a)

static int memsize;
chunk* firstchunk;


APType getChunkSize(chunk* c) {
	if(c->next) return PTR(c->next) - PTR(c);
	return PTR(memory)+PTR(memsize)-PTR(c);
}


void* allocmem(int size) {
	int chunksize = size+sizeof(chunk);

	/*align chunk to 16 byte boundary:*/
	chunksize = (chunksize & (~15)) + 16;

	chunk* c = firstchunk;

	void* found = null;

	while(c && !found) {
		APType targetChunkSize = getChunkSize(c);
		if(!c->taken && targetChunkSize > chunksize ) {
			/*if not taken, and big enough to hold data*/

			if(targetChunkSize > chunksize+128) {
				/*if chunk is over 128 bytes too big, split it*/
				chunk* newchunk = (chunk*)(PTR(c) + chunksize);
				newchunk->prev=c;
				newchunk->next=c->next;
				newchunk->taken=0;
				c->next=newchunk;
			}
			c->taken = 1;
			found = PTR(c)+(void*)sizeof(chunk);
		}
		c = c->next;
	}
	return found;
}

void freemem(void* ptr) {
	chunk* this = (chunk*)(PTR(ptr)-sizeof(chunk));

	this->taken=0;

	chunk* next = this->next;
	chunk* prev = this->prev;

	/* merge with prev chunk */
	if(!prev->taken) {
		prev = prev->prev;	/* prev is now the real previous chunk */
		prev->next = this;
		this->prev = prev;
	}

	//attempt to merge with next chunk
	if(!next->taken) {
		next = next->next;
		next->prev = this;
		this->next = next;
	}
}

void mm_init(void* memstart, int size) {
	memory = memstart;
	memsize = size;
	firstchunk = (chunk*)memory;
	allocmem(1);
	void* middle = allocmem(size-1000);
	allocmem(1);
	freemem(middle);
}
