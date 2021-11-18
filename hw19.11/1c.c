#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char** argv) {
  int fd1[2], fd2[2];
  dup2(0, fd2[0]);
  dup2(1, fd2[1]);
  for (int i = 1; i < argc; ++i) {
    if (i < argc - 1)
      i % 2 ? pipe(fd1) : pipe(fd2);
    else
      i % 2 ? dup2(1, fd1[1]) : dup2(1, fd2[1]);
    pid_t pid;
    pid = fork();
    switch(pid) {
      case -1 :
        perror(NULL);
        exit(errno);
      case 0 : // child
        if (i % 2) {
          dup2(fd1[1], 1);
          dup2(fd2[0], 0);
          close(fd2[1]);
          execlp(argv[i], argv[i], NULL);
        }
        else {
          dup2(fd2[1], 1);
          dup2(fd1[0], 0);
          close(fd1[1]);
          execlp(argv[i], argv[i], NULL);
        }
        exit(1);
      default :
        if (i % 2) {
         close(fd2[1]);
         close(fd2[0]);
       }
       else {
         close(fd1[1]);
         close(fd1[0]);
       }
        wait(NULL);
    }
  }
  return 0;
}
