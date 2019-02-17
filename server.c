#include <sys/types.h> // make fifo
#include <sys/stat.h> // also make fifo
#include <stdio.h>
#include <fcntl.h>  // open stuff
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

char SERVER_FIFO[] = "my_server_pipe";

void handle(int arg) {
	printf("\nHandling\n");
	unlink(SERVER_FIFO);
	exit(0); 
}

int main() {
	//const char SERVER_FIFO[] = "/tmp/my_server_pipe";
	char buf[2]; 
	bool done = false; 

	int result = mkfifo(SERVER_FIFO, 0600);
	if (result) {
		// unsuccessful
		perror("Unable to create named pipe");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, handle);
	printf("Opening file\n");
	int fd = open(SERVER_FIFO, O_RDONLY);
	if (fd == -1) {
		// didn't work
		perror("Unable to open file");
		exit(EXIT_FAILURE); 
	}
	printf("done opening file\n");

	while (!done) {
		result = read(fd, &buf, 1); 
		if ( result < 0 ) {
			perror("ERROR: Error reading from pipe");
			exit(EXIT_FAILURE);
		}
		else if ( result == 0 ) {
			// last client disconnected, EOF
			printf("End of file");
			done = true; 
		} else if ( '\a' == buf[0] ) {
			done = true; 
		} else if ( '\0' == buf[0] ) {
			printf("\n");
		} else {
			printf("%c", buf[0]);
		}
	}
	unlink(SERVER_FIFO);
	return 0; 
}
