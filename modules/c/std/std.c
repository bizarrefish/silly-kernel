#include <std/std.h>

int strlen(char* str) {
	int i = 0;
	while(*(str++)) i++;
	return i;
}

int strcpy(char* dest, char* src) {
	int i = 0;
	while(*src) {
		*(dest++) = *(src++);
		i++;
	}
	*dest = '\0';
	return i;
}

int strcmp(char* s1, char* s2) {
	int i = 0;
	while(*s1) {
		if(*s1 != *s2) i++;
		s1++;
		s2++;
	}
	return i;
}
