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

void conveyer(char** argv, int fd_in, int fd_out) {
  if (!argv[0])
    return;
  int count_ps = 0, left_lim = 0, right_lim, fd[2];
  for (int i = 0; argv[i]; ++i) {
    if (strcmp(argv[i], "|"))
      continue;
    ++count_ps;
    right_lim = i;
    pipe(fd);
    pid_t pid = fork();
    switch (pid) {
      case -1 :
        perror("fork");
        return;
      case 0 : // child
        dup2(fd_in, 0);
        dup2(fd[1], 1);
        free(argv[right_lim]);
        argv[right_lim] = NULL;
        simple_execution(argv + left_lim);
        exit(errno);
      default : // parent
        close(fd_in);
        fd_in = fd[0];
        left_lim = right_lim + 1;
        close(fd[1]);
    }
  }
  ++count_ps;
  if (count_ps == 1 && !strcmp(argv[0], "cd")) {
    simple_execution(argv);
    return;
  }
  pid_t pid = fork(); // last process
  switch (pid) {
    case -1 :
      perror("fork");
      return;
    case 0 : // child
      dup2(fd_in, 0);
      dup2(fd_out, 1);
      simple_execution(argv + left_lim);
      exit(errno);
    default : // parent
      close(fd_in);
      close(fd_out);
  }
  for (int i = 0; i < count_ps; ++i) {
    wait(NULL);
  }
  return;
}


char** command(char** argv) {
  int n, fd_in = -1;
  for(n = 0; argv[n] != NULL && strcmp(argv[n], "<"); ++n);
  if (argv[n] && argv[n + 1]) {
    fd_in = open(argv[n + 1], O_RDONLY);
    if (fd_in == -1)
      perror("open");
    argv = del_from_arr(argv, n, 2);
  }
  int fd_out = -1;
  for(n = 0; argv[n] != NULL && strncmp(argv[n], ">", 1); ++n); // search for a string starts with >
  if (argv[n] && argv[n + 1]) {
    if (!strcmp(argv[n], ">")) // single >
      fd_out = open(argv[n + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    else // double >
      fd_out = open(argv[n + 1], O_WRONLY | O_APPEND | O_CREAT, 0777);
    if (fd_out == -1)
      perror("open");
    argv = del_from_arr(argv, n, 2);
  }
  conveyer(argv, fd_in, fd_out);
  return argv;
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
  char** words_arr;
  int r, index;
  char* line;
  while (printf(">"), (line = read_line())) {
    index = 0;
    r = BASE1;
    words_arr = malloc(sizeof(char*) * BASE1);
    words_arr = separate(words_arr, line, &r, &index);
    free(line);
    char** arr_head = words_arr; // for free()
    words_arr = command(words_arr);
    for (int i = 0; words_arr[i]; ++i)
      free(words_arr[i]);
    free(arr_head);
  }
  putchar('\n');
  return 0;
}
