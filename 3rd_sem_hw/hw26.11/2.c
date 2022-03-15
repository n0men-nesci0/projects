#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
	int n;
	sscanf(argv[2], "%d", &n);
	int fd[2];
	pipe(fd);
	int fd0 = fd[0], fd1 = fd[1];
	for (int i = 0; i < n; ++i) {
		if (i < n - 1)
			pipe(fd);
		pid_t pid = fork();
		switch (pid) {
			case -1 :
				perror("fork");
				exit(1);
			case 0 : { // child
				close(fd1);
				int c;
				long long sum;
				for (int j = 0; read(fd0, &c, sizeof(int)) > 0; ++j)
					if (j % n == i) {
						sum += c;
						c = 0;
						write(fd[1], &c, sizeof(int));
					}
					else 
						write(fd[1], &c, sizeof(int));
				printf("process %d sum = %lld\n",getpid(), sum);
				close(fd[1]);
				close(fd0);
				exit(0);
			}
			default : 
				if (i < n - 1)
					close(fd[1]);
				close(fd0);
				fd0 = fd[0];
		}
	}
	int file = open(argv[1], O_RDONLY);
	int c;
	while (read(file, &c, sizeof(int)) > 0)
		write(fd1, &c, sizeof(int));
	/*	чтение fscanf'ом для более простой проверки
	FILE* f = fopen(argv[1], "r");
	while (fscanf(f, "%d", &c) > 0)
		write(fd1, &c, sizeof(int));
	*/
	close(fd1);
	close(fd0);
	for (int i = 0; i < n; ++i)
		wait(NULL);
	return 0;
}