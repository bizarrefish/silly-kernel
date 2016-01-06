#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	printf("Scancode table generator program\n");

	int file = open("codetable",O_RDWR|O_CREAT);

	int i = 0;
	while(1) {
		printf("scan code %x\n",i++);
		char c = getchar();
		if(c == 10) {
			c = 0;
			write(file,&c,1);
			continue;
		}
		if(c == '@') {
			break;
		}
		getchar();
		write(file,&c,1);
	}

	printf("Exiting\n");

	close(file);
	return 0;
}
