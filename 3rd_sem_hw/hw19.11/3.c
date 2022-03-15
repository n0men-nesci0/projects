#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <stdio.h>

int sig_int_count, sig_trap_count;

void sig_handler(int sig) {
  if (sig == SIGTRAP && sig_int_count < 4 && sig_int_count >= 2)
    ++sig_trap_count;
  else if(sig == SIGINT) {
    ++sig_int_count;
    if (sig_int_count == 7) {
      printf("%d\n", sig_trap_count);
      exit(0);
    }
  }
  return;
}

int main() {
  signal(SIGTRAP, sig_handler);
  signal(SIGINT, sig_handler);
  while(1);
  return 0;
}
