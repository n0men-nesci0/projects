#include <unistd.h>
#include <wait.h>
#include <stdio.h>

int main(int argc, char** argv) {
  int correct_ps = 0, os_err_ps = 0, prog_err_ps = 0;
  for (int i = 1; i < argc; ++i) {
    pid_t pid = fork();
    if (!pid) { // child
      execlp(argv[i], argv[i], NULL);
      return 1;
    }
    else if (pid == -1)
      ++os_err_ps;
  }
  for (int i = 1; i < argc; ++i) {
    int status;
    wait(&status);
    if (WIFEXITED(status))
      if (!WEXITSTATUS(status))
        ++correct_ps;
      else
        ++prog_err_ps;
    else
      ++os_err_ps;
  }
  printf("%d processes completed correctly\n%d processes completed with a mistake from the programmers point\n%d processes completed with a mistake from the os point\n", correct_ps, prog_err_ps, os_err_ps);
  return 0;
}
