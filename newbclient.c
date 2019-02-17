#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char SERVER_FIFO[] = "my_server_pipe";

int main() {
	bool done = false; 
	bool killserver = false; 
	char curr; 
	char* result;
	int errno;
	char buf[64];
	int fifo = open(SERVER_FIFO, O_WRONLY);
	if (fifo < 0) {
		perror("Unable to open named pipe");
		exit(-1);
	}
	int pid = getpid();
	sprintf(buf, "%d", pid);
//	while(!done) {
		/*result = fgets((char*)&buf, 64, stdin);
		if (result != NULL) {
			buf[strlen(buf)-1] == '\0';
			if (strcmp(buf, "exit") == 0) {
				done = true;
				killserver = true; 
			} else {
				printf("Sending %s\n", buf);
				errno = write(fifo, &buf, strlen(buf)+1);

				if (errno < 0) {
					perror("ERROR: Error writing to pipe");
				}	
			}
		} else {
			done = true; 
		}*/
		buf[strlen(buf)-1] = '\0';
		errno = write(fifo, &buf, strlen(buf)+1);
		if (errno <0) {
			perror("ERROR: Error writing to pipe");
		}
		//write(fifo, "\a", 1);
//	}
	if (killserver)
		write(fifo, "\a", 1);
	close(fifo);
	
	int fd = mkfifo("client_2_fifo", 0600);
	int fif = open("client_2_fifo", O_RDONLY);
	read(fif, &buf, 10);
	printf("%s\n", buf);
	while (strncmp(buf, "FIRE", 4) != 0) {
		read(fif, &buf, 10);
		printf("Waiting...\n");
	}
	printf("Fired\n");
	//unlink("client_1_fifo");
	printf("Click any button!\n");
	fifo = open(SERVER_FIFO, O_WRONLY);
	result = fgets((char*)&buf, 64, stdin);
	buf[strlen(buf)-1] = '\0';
	write(fifo, &buf, strlen(buf)+1);
	fif = open("client_2_fifo", O_RDONLY);
	read(fif, &buf, 10); 
	printf("%s\n", buf);
	//close(fifo);
	unlink("client_2_fifo");
	return 0;
}
