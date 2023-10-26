#include "./headers/comms.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

static int pipe_fd;

void handleSignal(int signo, siginfo_t *info, void *context) {
	// Read the message pointer from the signal info
	const char *message = (const char*)info->si_value.sival_ptr;
	
	// Write the message to the pipe
	write(pipe_fd, message, strlen(message) + 1);
}

void initCommunication() {
	// Create a named pipe
	mkfifo("commPipe", 0666);
	
	//Open the pipe for writing
	pipe_fd = open("commPipe", O_WRONLY);
}

void sendMessage(pid_t receiver_pid, const char *message) {
	union sigval sv;
	sv.sival_ptr = (void*)message;
	
	if(sigqueue(receiver_pid, SIGUSR1, sv) == -1) {
		perror("Error sending signal");
		exit(EXIT_FAILURE);
	}
}




