#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>

char SERVER_FIFO[] = "my_server_pipe";

void handle(int arg) {
	printf("\nHandling...\n");
//	unlink(SERVER_FIFO);
	exit(0);
}

int main() {
	//const char SERVER_FIFO[] = "/tmp/my_server_pipe";
	bool done = false;
	bool killserver = false; 
	char curr; 
	char* result;
	int errno;
	char buf[64];
	
	int fifo = open(SERVER_FIFO, O_RDWR);
	if ( fifo < 0 ) {
		perror("Unable to open named pipe");
		exit(-1);
	}

	signal(SIGINT, handle);
	bool fun = false; 
	int reader; 
	int count = 0; 
	while ( !fun && count < 3 ) {
		reader = read(fifo, &buf, 1);
		if (reader < 0) {
			perror("ERROR: can't read from pipe");
			exit(EXIT_FAILURE);
		} else if (reader == 0) {
			printf("End of file");
			fun = true; 
		} else if ('\a' == buf[0] ) {
			fun = true; 
		} else if ('\0' == buf[0] ) {
			printf("\n");
			printf("Received: %s\n", buf); 
			count++;
		} else {
			printf("%c", buf[0]);
		}
	}
	while ( !done ) {
		result = fgets((char*)&buf, 64, stdin);
		if ( result != NULL ) {
			buf[strlen(buf)-1] = '\0';
			if ( strcmp(buf, "exit") == 0 ) {
				done = true; 
				killserver = true; 
			} else {
				printf("Sending %s\n", buf);
				errno = write(fifo, &buf, strlen(buf) + 1);

				if ( errno < 0 ) {
					perror("ERROR: Error writing to pipe");
				}
			}
		} else {
			done = true; 
		}
	}
	if ( killserver ) {
		write(fifo, "\a", 1);
	}
	return 0; 
}
