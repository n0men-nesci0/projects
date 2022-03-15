#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>

int target_pid, count;
int fd[2];
int status;

void sig_hndlr(int signal) {
	if (signal == SIGUSR1) {
		read(fd[0], &count, sizeof(int));
		printf("%d\n", count);
		++count;
		write(fd[1], &count, sizeof(int));
		kill(target_pid, SIGUSR1);
	}
	else if (target_pid == getppid()) { // child
		printf("terminating child\n");
		close(fd[1]);
		close(fd[0]);
		exit(0);	
	}
	else {
		kill(target_pid, SIGINT);
	}
}

int main() {
	signal(SIGINT, sig_hndlr);
	signal(SIGUSR1, sig_hndlr);
	pipe(fd);
	if (target_pid = fork()) {
		write(fd[1], &count, sizeof(int));
		while (wait(&status) == -1);
		printf("terminating parent\n");
		close(fd[1]);
		close(fd[0]);
		return 0;
	}
	else {
		read(fd[0], &count, sizeof(int));
		target_pid = getppid();
		write(fd[1], &count, sizeof(int));
		kill(target_pid, SIGUSR1);
		while(1)
			pause();
	}
}