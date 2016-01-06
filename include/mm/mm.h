/* header for memory allocator */

struct header {
	struct header* next;
	struct header* prev;
	int taken;
};

typedef struct header chunk;


void mm_init(void* memstart, int size);

void freemem(void* ptr);

void* allocmem(int size);

void* reallocmem(void* ptr,void* size); /*TODO*/
