#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <wait.h>
#include <errno.h>

//pr1 < f1.dat | pr2 > f2.dat; pr3

int main(int argc, char** argv) {
    int fd[2];
    pipe(fd);
    if (!fork()) { // child pr1
      int fd_in = open(argv[2], O_RDONLY);
      dup2(fd_in, 0);
      dup2(fd[1], 1);
      close(fd_in);
      close(fd[1]);
      close(fd[0]);
      execlp(argv[1], argv[1], NULL); // pr1(arg1,arg2)->fd[1]
    }
    else // parent
      if (!fork()) { // child pr2
        int fd_out = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
        dup2(fd_out, 1);
        dup2(fd[0], 0);
        close(fd_out);
        close(fd[0]);
        close(fd[1]);
        execlp(argv[3], argv[3], NULL); // pr2<-fd[0]
      }
    // parent
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
    execlp(argv[5], argv[5], NULL);
}
