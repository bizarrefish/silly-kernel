                                                                     
                                                                     
                                                                     
                                             
/* list macro header */

/* Set of macros to provide circular doubly list functionality */

/* List must be 1 or more elements in size */

/* Lists are composed of structures typedef-ed to "Node" */

/* Structures must contain a "next" and "prev" pointer */

/* The initial node must point to itself */


/* for-block to iterate through nodes, takes pointer to start node, and name of iterator pointer */
#define forEachNode(start,this) int firstrun = 2; \
	for(this=(start);(firstrun=firstrun>>1)||(this!=(start));this=nextNode(this))


/* returns the node after/before the one given */
#define nextNode(this) this->next
#define prevNode(this) this->prev

/* adds a node after the current one */
#define addNodeAfter(_this,new) \
	Node* this = _this;\
	Node* next = this->next;\
	this->next = new;\
	next->prev = new;\
	new->next = next;\
	new->prev = this;

/* adds a node before the current one */
#define addNodeBefore(this,new) addNodeAfter((this)->prev,new);

/* removes the current node from it's list */
#define removeFromList(del) \
	Node* after = del->next;\
	Node* before = del->prev;\
	after->prev = before;\
	before->next = after;\
	del->next = del;\
	del->prev = del;

