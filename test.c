#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

char flag[512];
char real_key[] = "65b42f7d71f9fb31d824c28b8943ad94";
char tmp[32] = "/tmp/";

void myalarm()
{
exit(-1);
}

int my_read(int fd, char * dest, int size) {
	int a;
	a = read(fd, dest, size);	
	dest[a-1] = '\0';
	return 0;
}

int gadget() {

asm("pop %rdi");
asm("pop %rsi");
asm("pop %rdx");
asm("pop %rcx");
asm("pop %r8");
asm("pop %r9");
asm("ret");

}


int closeFD() {
	int i;
	int flag=0;
	for (i=0; i<=2; i++) {
		if (close(i)){
			flag=0x12345678;
		}
	}
	return flag;	
}

int main () {
	char buf[256];
	char key[32];
	char id[16];
	float rand_num;
	FILE *fp;
	setvbuf(stdout, 0,2,0);
	setvbuf(stdin, 0,1,0);
	signal(SIGALRM, myalarm);
	alarm(0x10);

	srand(time(0));

	fp = open("flag", 'r');
	read(fp, flag, 32);
	close(fp);


	puts("Hello!");
	printf("Input ID: ");
	my_read(0, id, 15);

	strcat(tmp, id);		

	
	if (!stat(tmp)) {
		fp = open(tmp, 'r');
		read(fp, buf, 2);
		close(fp);
		if (!strcmp(buf, "on")) {
			puts("No race condition");
			exit(-1);
		}
		fp = open(tmp, O_WRONLY);
		write(fp, "on", 2);
		close(fp);
	} else {
		fp = open(tmp, O_WRONLY | O_CREAT, 0644);
		write(fp, "on", 2);
		close(fp);
	}

	printf("Input key: ");
	my_read(0, key, 33);

	printf("Check Key");
	sleep(1);
	printf(".");
	sleep(1);
	printf(".");
	printf("\n");

	if (strncmp(key, real_key, 32)) {
		puts("Wrong ...");
		fp = open(tmp, O_WRONLY, 0444);
		write(fp, "off", 3);
		close(fp);
		exit(-1);
	}

	printf("Welcome: ");
	my_read(0, buf, 512);

	printf("If close failed.. you will sleep forever\n");
	sleep(closeFD());	

	
	fp = open(tmp, O_WRONLY);
	write(fp, "off", 3);
	close(fp);
	return 0;	
}
