#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

char SERVER_FIFO[] = "my_server_pipe";

int main() {
	char buf[2]; 
	bool done = false; 
	char *client_pid; 

	int result = mkfifo(SERVER_FIFO, 0600);
	if (result) {
		perror("unable to create named pipe");
		exit(EXIT_FAILURE);
	}

	printf("Opening file\n");
	int fd = open(SERVER_FIFO, O_RDONLY);
	if (fd == -1) {
		perror("Unable to create named pipe");
		exit(EXIT_FAILURE);
	}
	printf("done opening file\n");
	int numClients = 0; 
	while(!done) {
		if (numClients == 2) {
			done = true; 
		}
		result = read(fd, &buf, 1);
		client_pid = buf;
		if (result < 0) {
			perror("ERROR: Error reading from pipe");
			exit(EXIT_FAILURE);
		} else if (result == 0) {
			printf("EOF\n");
			done = true; 
		} else if ('\a' == buf[0]) {
			done = true; 
		} else if ('\0' == buf[0]) {
			printf("\n");
			numClients++;
		} else {
			printf("%c", buf[0]);
		}
	}
//	unlink(SERVER_FIFO);
	close(fd);
	clock_t t; 
//	int client1_fifo = open("client_1_fifo", O_WRONLY);
	int client2_fifo = open("client_2_fifo", O_WRONLY); 
//	write(client1_fifo, "ready", strlen("ready")+1);
	write(client2_fifo, "ready", strlen("ready")+1);
	sleep(3);
//	write(client1_fifo, "FIRE", strlen("FIRE")+1);
	write(client2_fifo, "FIRE", strlen("FIRE")+1);
	fd = open(SERVER_FIFO, O_RDONLY);
	//sleep(2);
	t = clock();
	read(fd, &buf, 10);
	t = clock() - t;
	double time = ((double)t)/CLOCKS_PER_SEC; 
	printf("Received %s in %f seconds\n", buf, time);
//	write(client1_fifo, "you won!", strlen("you won!")+1);
	write(client2_fifo, "you won!", strlen("you won!")+1);
	unlink(SERVER_FIFO);
	return 0; 
}
