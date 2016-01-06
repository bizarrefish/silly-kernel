#include <video/video.h>
#include <process/process.h>

#define vidAddress 0xB8000
#define vidLines 25
#define vidCols 80


//ASCII from number
#define toASCII(x) (char)(x+0x30)

typedef struct { char cols[vidCols*2]; } DisplayLine;
DisplayLine vidBuffer[vidLines];

typedef struct { DisplayLine lines[vidLines]; } VideoMemory;
static VideoMemory* vidptr = (VideoMemory*)vidAddress;


int currentBufferLine = 0;	/* selected DisplayLine to write to in vidBuffer */
int currentBufferColumn = 0;	/* 0 -> vidCols*2-1 */
#define nextLine(line) ((line+1)%vidLines)




static void _printChar(char c) {
	vidBuffer[currentBufferLine].cols[currentBufferColumn] = c;
	vidBuffer[currentBufferLine].cols[currentBufferColumn+1] = '\a';
	currentBufferColumn+=2;
}


static void update() {	/* copy buffer to video memory */
	int srcLine = nextLine(currentBufferLine);
	int destLine = 0;
	int i = vidLines*vidCols*2;
	/* For each line */
	while(destLine < vidLines) {
		
		/* for each column */
		int c; for(c = 0; c < vidCols*2; c++) {
			char ch = vidBuffer[srcLine].cols[c];
			(*vidptr).lines[destLine].cols[c] = ch;
		}
		srcLine = nextLine(srcLine);
		destLine++;
	}
}




static void LFCR() {
	for(; currentBufferColumn <= vidCols*2; currentBufferColumn+=2) {
		vidBuffer[currentBufferLine].cols[currentBufferColumn] = ' ';
		vidBuffer[currentBufferLine].cols[currentBufferColumn+1] = '\a';
	}
	currentBufferColumn = 0;
	currentBufferLine = nextLine(currentBufferLine);
	int i;
	for(i = 0; i < vidCols*2; i+=2) {
		vidBuffer[currentBufferLine].cols[i] = ' ';
		vidBuffer[currentBufferLine].cols[i+1] = '\a';
	}
	update();
}



/* Print char at current location */
void printChar(char c) {
	if(c == '\n'){ LFCR(); return; }
	if(currentBufferColumn > vidCols*2) LFCR();
	

	/* If this is the first char of the line, print the proc name at the front */
	if(currentBufferColumn == 0) {
		char* name = getCurrentProcName();
		while(*name) _printChar(*(name++));
		_printChar('>');
		_printChar(' ');
	}

	_printChar(c);
}


void printNum(unsigned int number,int col) {		//prints out decimal value
	int subt = 0;		//value to subtract
	while(col) {
		int digit = (number/col)-subt;
		subt+=digit;
		subt*=10;
		col/=10;
		printChar(toASCII(digit));
	}
}

void printString(char* str) {
	while(*str) printChar(*(str++));
}

void CLS() {
	int i = 0;
	int j = 0;
	while(i++ < vidLines) {
		while(j++ < vidCols) {
			printChar(' ');
		}
		printChar('\n');
	}
	currentBufferLine = 0;
	currentBufferColumn = 0;
}

void printHexInt(unsigned int i) {
	printString("0x");
	unsigned int number;
	int shift; for(shift = 7; shift > -1;shift--) {
		number = (i >> shift*4) & 0xF;
		if(number < 10)	printChar(toASCII(number));
		else printChar((char)(number+0x37));
	}		
}













/*
static void LFCR() {
	int cellNum = (vidptr - (char*)vidAddress)/2;
	int line = cellNum/vidCols;
	line++;
	vidptr = ((char*)vidAddress)+line*vidCols*2;
}

void printChar(char c) {
	if(c == '\n'){ LFCR(); return; }
	int vidoffset = (int)vidptr - (int)vidAddress;
	vidoffset = (vidoffset + 2) % (vidLines*vidCols*2);

	if(vidoffset > ((vidLines-1)*vidCols*2)) CLS();
	*vidptr = c;

	vidptr = (char*)vidAddress + vidoffset;
}


void printNum(unsigned int number,int col) {		//prints out decimal value
	int subt = 0;		//value to subtract
	while(col) {
		int digit = (number/col)-subt;
		subt+=digit;
		subt*=10;
		col/=10;
		printChar(toASCII(digit));
	}
}

void printString(char* str) {
	while(*str) printChar(*(str++));
}

void CLS() {
	vidptr = (char*)vidAddress;
	int i; for(i = 0; i < vidLines*vidCols; i++) {
		vidptr[i*2] = (char)0;
	}

	vidptr = (char*)vidAddress;
}

void printHexInt(unsigned int i) {
	printString("0x");
	unsigned int number;
	int shift; for(shift = 7; shift > -1;shift--) {
		number = (i >> shift*4) & 0xF;
		if(number < 10)	printChar(toASCII(number));
		else printChar((char)(number+0x37));
	}		
}
*/
