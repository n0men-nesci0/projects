#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <stdio.h>

int number = 1;

void sig_handler(int sig) {
    ++number;
    return;
}

int main() {
  printf("ctrl + C to increase number by 1\nctrl + \\ to Quit\n");
  signal(SIGINT, sig_handler);
  while(1) {
    sleep(1);
    printf("%d\n", number);
  }
  return 0;
}
