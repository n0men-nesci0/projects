#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

// (CMD1 && CMD2) | (CMD3; CMD4) > FILE

int exec_with_fd(char* cmd, int fd0, int fd1) {
  int status;
  pid_t pid = fork();
  switch (pid) {
    case -1 :
      perror(NULL);
      exit(errno);
    case 0 : // child
        dup2(fd0, 0);
        dup2(fd1, 1);
        close(fd1);
      execlp(cmd, cmd, NULL);
      perror(NULL);
      exit(errno);
    default : // parent
      wait(&status);
    }
  return status;
}

int main(int argc, char** argv) {
  int fd[2];
  pipe(fd);
  int status = exec_with_fd(argv[1], dup(0), fd[1]);
  if (!WIFEXITED(status) || WEXITSTATUS(status))
    return 1;
  exec_with_fd(argv[2], dup(0), fd[1]);
  close(fd[1]);
  int file = open(argv[5], O_CREAT | O_WRONLY | O_TRUNC, 0777);
  exec_with_fd(argv[3], fd[0], file);
  exec_with_fd(argv[4], fd[0], file);
  close(fd[0]);
  return 0;
}
