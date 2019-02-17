#include <sys/types.h> // make fifo
#include <sys/stat.h> // also make fifo
#include <stdio.h>
#include <fcntl.h>  // open stuff
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>

char SERVER_FIFO[] = "my_server_pipe";
char CLIENT_FIFO[] = "my_client_pipe";

void handle(int arg) {
	printf("\nHandling\n");
	unlink(SERVER_FIFO);
	unlink(CLIENT_FIFO);
	exit(0); 
}

int main() {
	//const char SERVER_FIFO[] = "/tmp/my_server_pipe";
	char buf[2];
	bool done = false; 
	char buffer[64];

	int result = mkfifo(SERVER_FIFO, 0600);
//	int result2 = mkfifo(CLIENT_FIFO, 0600);

	if (result) {
		// unsuccessful
		perror("Unable to create named pipe");
		exit(EXIT_FAILURE);
	} 
/*	if (result2) {
		perror("Unable to create named pipe");
		exit(EXIT_FAILURE);
	}*/

	signal(SIGINT, handle);
	printf("Opening file\n");
	int fd = open(SERVER_FIFO, O_RDWR);

	if (fd == -1) {
		// didn't worki
		perror("Unable to open file");
		exit(EXIT_FAILURE); 
	}
	char* results; 
	printf("done opening file\n");
	while (!done) {
		int errno = 0; 
	 	results = fgets((char*)&buffer, 64, stdin);	
		if(results != NULL) {
			buffer[strlen(buffer)-1] = '\0';
			printf("Sending %s\n", buffer);
			errno = write(fd, &buffer, strlen(buffer) + 1);
			if (errno < 0) {
				perror("erorr writing to fifo");
				
			}
		}
		int num = (rand() % (5 - 2 + 1)) + 2; 
		sleep(num);
		results = fgets((char*)&buffer, 64, stdin);
		if(results != NULL) {
			buffer[strlen(buffer)-1] = '\0';
			printf("Sending %s\n", buffer);
			errno = write(fd, &buffer, strlen(buffer) + 1);
			if (errno<0) {
				perror("error writing to fifo");
			}
		}
		done = true;
}
	sleep(5);
	done = false; 
	while (!done) {
		result = read(fd, &buf, 1);
		if ( result < 0 ) {
			perror("ERROR: Error reading from pipe");
			exit(EXIT_FAILURE);
		} else if ( result == 2 ) {
			printf("2 clients\n");
		}
		else if ( result == 0 ) {
			// last client disconnected, EOF
			printf("End of file");
			done = true; 
		} else if ( '\a' == buf[0] ) {
			done = true; 
		} else if ( '\0' == buf[0] ) {
			printf("\n");
			printf("Received: %s\n", buf);
		} else {
			printf("%c", buf[0]);
		}
	}
	unlink(SERVER_FIFO);
	return 0; 
}
