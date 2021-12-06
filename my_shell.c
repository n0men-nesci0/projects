#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define STEP 15
#define BASE1 10
#define BASE2 30
#define FACTOR 1.5
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_DEFAULT "\x1b[0m"

int bg_flag = 0;

void print_arr(char** words_arr) {
  for (; *words_arr; ++words_arr)
    printf("%s\n", *words_arr);
  return;
}

char** del_from_arr(char** argv, int pos, int num) {
  int length = 0;
  for(; argv[length]; ++length);

  for (int i = 0; i < num; ++i) // free strings we want to delete
    free(argv[pos + i]);

  if (length - pos - num < pos) { // which end of array is shorter and easier to move
    memmove(argv + pos, argv + pos + num, (length - pos - num) * sizeof(char*));
    argv[length - num] = NULL;
    return argv;
  }
  else {
    memmove(argv + num, argv, pos * sizeof(char*));
    return argv + num;
  }
}

void simple_execution(char** argv) {
  if (strcmp(argv[0], "cd")) {
    execvp(argv[0], argv);
    if (!bg_flag)
      perror("exec");
    close(0);
    close(1);
    exit(errno);
  }
  else {
    int exit_status;
    if (argv[1])
      exit_status = chdir(argv[1]);
    else
      exit_status = chdir(getenv("HOME"));
    if (exit_status < 0)
      perror(NULL);
  }
  return;
}

int command(char** argv, int* count_ps) {
  int left_lim = 0, right_lim, fd0 = -1, fd1 = -1, fd[2], exit_status = 0;
  if (bg_flag)
    fd0 = open("/dev/null", O_RDWR);
  for(int i = 0; argv[i]; ++i) {
    if (!strcmp(argv[i], "|")) {
      right_lim = i;
      if (!argv[left_lim])
        continue;
      if (fd1 < 0) {
        pipe(fd);
        fd1 = fd[1];
      }
      else
        fd[0] = -1;
      pid_t pid = fork();
      switch (pid) {
        case -1 :
          perror("fork");
          return errno;
        case 0 : // child
          if (bg_flag)
            setpgrp();
          else
            signal(SIGINT, SIG_DFL);
          dup2(fd0, 0);
          dup2(fd1, 1);
          free(argv[right_lim]);
          argv[right_lim] = NULL;
          simple_execution(argv + left_lim);
          exit(errno);
        default : // parent
          printf("[%d]\t%d\n", *count_ps, pid);
          close(fd0);
          fd0 = fd[0];
          left_lim = right_lim + 1;
          close(fd1);
          fd1 = -1;
      }
      ++(*count_ps);
    }
    if (!strcmp(argv[i], "<")) {
      if (!argv[i + 1] || !strcmp(argv[i], "|")) {
        printf("WRONG COMMAND\n");
      }
      else {
        close(fd0);
        fd0 = open(argv[i + 1], O_RDONLY);
        if (fd0 == -1)
          perror("open");
        argv = del_from_arr(argv, i, 2);
        --i;
      }
    }
    if (!strncmp(argv[i], ">", 1)) {
      if (!argv[i + 1] || !strcmp(argv[i], "|")) {
        printf("WRONG COMMAND\n");
      }
      else {
        if (!strcmp(argv[i], ">")) // single >
          fd1 = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        else // double >
          fd1 = open(argv[i + 1], O_WRONLY | O_APPEND | O_CREAT, 0666);
        if (fd1 == -1)
          perror("open");
        argv = del_from_arr(argv, i, 2);
        --i;
      }
    }
  }
  ++(*count_ps);
  if (*count_ps == 1 && !strcmp(argv[0], "cd") && !bg_flag)
    return errno;
  if (bg_flag && fd1 < 0)
      fd1 = open("/dev/null", O_RDWR);
  pid_t pid = fork(); // last process
  switch (pid) {
    case -1 :
      perror("fork");
      return errno;
    case 0 : // child
      if (bg_flag)
        setpgrp();
      else
        signal(SIGINT, SIG_DFL);
      dup2(fd0, 0);
      dup2(fd1, 1);
      simple_execution(argv + left_lim);
      exit(errno);
    default : // parent
      close(fd0);
      close(fd1);
  }
  for (int i = 0; argv[i]; ++i)
    free(argv[i]);
  if (!bg_flag) {
    for (int i = 0; i < *count_ps - 1; ++i) {
      waitpid(0, NULL, 0);
    }
    waitpid(pid, &exit_status, 0);
  }
  return WEXITSTATUS(exit_status);
}

int conditional_command(char** argv) {
  int left_lim = 0, right_lim, count_ps = 0;
  for (int i = 0; argv[i]; ++i) {
    if (!strcmp(argv[i], "&&")) {
      right_lim = i;
      free(argv[right_lim]);
      argv[right_lim] = NULL;
      int exit_status = command(argv + left_lim, &count_ps);
      if (exit_status) {
        for (++i; argv[i]; ++i)
          free(argv[i]);
        return count_ps;
      }
      left_lim = right_lim + 1;
    }
    else if (!strcmp(argv[i], "||")) {
      right_lim = i;
      free(argv[right_lim]);
      argv[right_lim] = NULL;
      int exit_status = command(argv + left_lim, &count_ps);
      if (!exit_status) {
        for (++i; argv[i]; ++i)
          free(argv[i]);
        return count_ps;
      }
      left_lim = right_lim + 1;
    }
    else if (!strcmp(argv[i], ";")) {
      right_lim = i;
      free(argv[right_lim]);
      argv[right_lim] = NULL;
      command(argv + left_lim, &count_ps);
      left_lim = right_lim + 1;
    }
  }
  if (argv[left_lim])
    command(argv + left_lim, &count_ps);
  return count_ps;
}

int shell_command(char** argv) {
  int left_lim = 0, right_lim, bg_ps = 0;
  for (int i = 0; argv[i]; ++i) {
    if (!strcmp(argv[i], "&")) {
      right_lim = i;
      if (!argv[left_lim])
        continue;
      free(argv[right_lim]);
      argv[right_lim] = NULL;
      bg_flag = 1;
      bg_ps += conditional_command(argv + left_lim);
      bg_flag = 0;
      left_lim = right_lim + 1;
    }
  }
  if (argv[left_lim])
    conditional_command(argv + left_lim);
  return bg_ps;
}

char* read_line() {
  char* str = malloc(BASE2);
  int c, i, r = BASE2;
  for (i = 0; (c = getchar()) != '\n' && c != EOF; ++i, --r) {
    if (!r) {
      r = i * (FACTOR - 1);
      str = realloc(str, i + r);
    }
    str[i] = c;
  }
  if (!i && c == EOF) {
    free(str);
    return NULL;
  }
  str = realloc(str, i + 1);
  str[i] = 0;
  return str;
}

char** add_to_arr(char** words_arr, char* word, int* r_ptr, int* index_ptr) {
  if (!(*r_ptr)) { // no memory left
    *r_ptr = *index_ptr * (FACTOR - 1);
    words_arr = realloc(words_arr, (*index_ptr + *r_ptr) * sizeof(char*));
  }
  words_arr[*index_ptr] = word;
  ++(*index_ptr);
  --(*r_ptr);
  return words_arr;
}

char** separate(char** words_arr, char* line, int* r_ptr, int* index_ptr) { // r means number of remaining slots
  int next_word, ins_quotes = 0, r_wrd = 0;
  char* word= NULL, *sprt = "&|<>;()", *double_sprt = "&|>";
  for (int i = 0, j = 0; ; ++i) {
    int c = line[i];
    next_word = !c || (!ins_quotes && (isspace(c) || strchr(sprt, c)));
    if (!next_word) {
      if (c == '"') {
        ins_quotes = !ins_quotes;
        continue;
      }
      if (!r_wrd) { // no memory left
        word = realloc(word, j + STEP);
        r_wrd = STEP;
      }
      word[j] = c;
      ++j;
      --r_wrd;
    }
    else {
      if (word) {
        word = realloc(word, j + 1);
        word[j] = 0;
        words_arr = add_to_arr(words_arr, word, r_ptr, index_ptr);
        word = NULL;
        j = 0;
        r_wrd = 0;
      }
      if (strchr(sprt, c) && c) { // special symbol
        word = malloc(3);
        word[0] = c;
        j = 1;
        if (strchr(double_sprt, c) && c == line[i + 1]) {
          word[j] = line[i+1];
          ++j;
          ++i;
        }
        word[j] = 0;
        words_arr = add_to_arr(words_arr, word, r_ptr, index_ptr);
        word = NULL;
        j = 0;
      }
    }
    if (!c) {
      words_arr = add_to_arr(words_arr, NULL, r_ptr, index_ptr);
      return words_arr;
    }
  }
}

int main() {
  signal(SIGINT, SIG_IGN);
  char** words_arr;
  int r, index, bg_ps = 0;
  char* line;
  char* cur_dir;
  while (printf("%s%s%s>", COLOR_RED, cur_dir = getcwd(NULL, 0), COLOR_DEFAULT), (line = read_line())) {
    free(cur_dir);
    index = 0;
    r = BASE1;
    words_arr = malloc(sizeof(char*) * BASE1);
    words_arr = separate(words_arr, line, &r, &index);
    free(line);
    bg_ps += shell_command(words_arr);
    for (int i = 0; i < bg_ps; ++i) {
      int status;
      pid_t pid = waitpid(-1, &status, WNOHANG);
      if (pid > 0) {
        if (WIFEXITED(status)) {
          int exit_status = WEXITSTATUS(status);
          printf("background process with pid %d exit with status %d\n", pid, exit_status);
          if (exit_status)
            printf("ERROR : %s\n", strerror(exit_status));
        }
        else if (WIFSIGNALED(status))
          printf("background process with pid %d killed by signal number %d\n", pid, WTERMSIG(status));
        --bg_ps;
        --i;
      }
    }
    free(words_arr);
  }
  free(cur_dir);
  putchar('\n');
  return 0;
}
