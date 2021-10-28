#include <unistd.h>
#include <wait.h>
#include <stdio.h>

int main(int argc, char** argv) {
  for (int i = 1; i < argc; ++i) {
    if (fork() > 0) { // parent
      int status;
      pid_t pid;
      pid = wait(&status);
      if (WIFEXITED(status))
        printf("process number %d with pid %d finished with status %d\n", i, pid, WEXITSTATUS(status));
    }
    else { // child
      execlp(argv[i], argv[i], NULL);
      return -1;
    }
  }
  return 0;
}
