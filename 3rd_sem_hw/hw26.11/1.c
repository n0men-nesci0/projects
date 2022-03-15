#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>

void sig_hndlr(int signal) {
	printf("time limit exceeded\n");
	exit(1);
}

int main(int argc, char** argv) {
	alarm(2);
	signal(SIGALRM, sig_hndlr);
	int fd[2];
	pipe(fd);
	pid_t pid = fork();
	switch (pid) {
		case -1 :
			perror("fork");
			exit(1);
		case 0 : // child
			dup2(fd[1], 1);
			execlp("ls", "ls", "-l", argv[1], NULL);
			perror("exec");
			exit(2);
		default : {
			close(fd[1]);
			char str[125];
			int i;
			for (i = 0; read(fd[0], str + i, 1) > 0; ++i) {
				if (str[i] == '\n') {
					str[i] = 0;
					if (str[0] == 'd') {
						for (; str[i] != ' '; --i);
						printf("%s\n", str + i + 1);
					}
					i = -1;
				}
			}
		}
	}
	wait(NULL);
	return 0;
}