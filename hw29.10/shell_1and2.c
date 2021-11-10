#include <unistd.h>
#include <wait.h>
#include <stdio.h>

int main(int argc, char** argv) {
  if (argc != 3) return 3;
  if (!fork()) {
    execlp(argv[1], argv[1], NULL);
    return 1;
  }
  int status;
  wait(&status);
  if (WIFEXITED(status) && !WEXITSTATUS(status))
    if (!fork()) {
      execlp(argv[2], argv[2], NULL);
      return 2;
    }
  wait(NULL);
  return 0;
}
