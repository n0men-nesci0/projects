#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <wait.h>
#include <errno.h>

//pr1 arg1 arg2 | pr2; pr3 >> f1.dat

int main(int argc, char** argv) {
    int fd[2];
    pipe(fd);
    if (!fork()) { // child pr1
      dup2(fd[1], 1);
      close(fd[1]);
      close(fd[0]);
      execlp(argv[1], argv[1], argv[2], argv[3], NULL); // pr1(arg1,arg2)->fd[1]
    }
    else // parent
      if (!fork()) { // child pr2
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execlp(argv[4], argv[4], NULL); // pr2<-fd[0]
      }
    // parent
    close(fd[0]);
    close(fd[1]);
    int fd1 = open(argv[6], O_WRONLY | O_APPEND | O_CREAT, 0644);
    wait(NULL);
    wait(NULL);
    dup2(fd1, 1);
    close(fd1);
    execlp(argv[5], argv[5], NULL);
}
