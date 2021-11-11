#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STEP 15
#define BASE1 10
#define BASE2 30
#define FACTOR 1.5

void keys_processing(int argc, char** argv, char** f_i_nm_ptr, char** f_o_nm_ptr) {
  for (int i = 1; i < argc; ++i)
    if (!strcmp(argv[i], "-i")) {
      if (i + 1 >= argc || argv[i+1][0] == '-')
        fprintf(stderr, "WARNING : can't find input file name. Set default\n");
      else *f_i_nm_ptr = argv[++i];
    }
    else if (!strcmp(argv[i], "-o")) {
      if (i + 1 >= argc || argv[i+1][0] == '-')
        fprintf(stderr,"WARNING : can't find output file name. Set default\n");
      else *f_o_nm_ptr = argv[++i];
    }
    else {
      fprintf(stderr, "FATAL ERROR : can't process argument %s\n", argv[i]);
      exit(0);
    }
  return;
}

void open_files(char* f_in_name, char* f_out_name, FILE** f_in_ptr, FILE** f_out_ptr) {
  if (f_in_name)
    *f_in_ptr = fopen(f_in_name, "r");
  if (!*f_in_ptr) {
    fprintf(stderr, "FATAL ERROR : can't find input file\n");
    exit(0);
  }
  if (f_out_name)
    *f_out_ptr = fopen(f_out_name, "w");
  return;
}

int is_sprt_ch(int c) {
  return c == '&' || c == '|' || c == ';' || c == '>' || c == '<' || c == '(' || c == ')';
}

char* read_line(FILE* f) {
  char* str = malloc(BASE2);
  int c, i, r = BASE2;
  for (i = 0; (c = fgetc(f)) != '\n' && c != EOF; ++i, --r) {
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

char** separate(char** words_arr, char* line, int* r_ptr, int* index_ptr) {
  int next_word, ins_quotes = 0, r_wrd = 0;
  char* word= NULL;
  for (int i = 0, j = 0; ; ++i) {
    int c = line[i];
    next_word = !c || !ins_quotes && (isspace(c) || is_sprt_ch(c));
    if (!next_word) {
      if (c == '"') {
        ins_quotes = !ins_quotes;
        continue;
      }
      if (!r_wrd) {
        word = realloc(word, j + STEP);
        r_wrd = STEP;
      }
      word[j] = c;
      ++j;
      --r_wrd;
    }
    else if (word) {
      word = realloc(word, j + 1);
      word[j] = 0;
      if (!(*r_ptr)) {
        *r_ptr = *index_ptr * (FACTOR - 1);
        words_arr = realloc(words_arr, (*index_ptr + *r_ptr) * sizeof(char*));
      }
      words_arr[*index_ptr] = word;
      word = NULL;
      ++(*index_ptr);
      --(*r_ptr);
      j = 0;
      r_wrd = 0;
    }
    if (!c)
      return words_arr;
  }
}

void print_arr(char** words_arr, char** arr_end, FILE* f_out) {
  for (; words_arr < arr_end; ++words_arr)
    fprintf(f_out, "%s\n", *words_arr);
  return;
}

void free_all(char** words_arr, int size) {
  for (int i = 0; i < size; ++i)
    free(words_arr[i]);
  free(words_arr);
}

int main(int argc, char** argv) {
  char *f_in_name = NULL, *f_out_name = NULL;
  keys_processing(argc, argv, &f_in_name, &f_out_name);
  FILE *f_in = stdin, *f_out = stdout;
  open_files(f_in_name, f_out_name, &f_in, &f_out);

  char** words_arr = malloc(sizeof(char*) * BASE1);
  int r = BASE1, index = 0;
  char* line;
  while (line = read_line(f_in)) {
    words_arr = separate(words_arr, line, &r, &index);
    free(line);
  }
  print_arr(words_arr, words_arr + index, f_out);
  free_all(words_arr, index);
  return 0;
}
