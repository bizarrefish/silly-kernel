/* video header file */

void printChar(char c);

void printNum(unsigned int i,int col);
#define printInt(x) printNum((unsigned int)x,1000000000)
#define printByte(x) printNum((unsigned int)x,100)

void printString(char* str);

void printHexInt(unsigned int i);

void CLS();
