#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define STEP 10

typedef struct binary_tree {
  struct binary_tree *left, *right;
  char *word;
  int count;
} tree;

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

char* read_word(FILE* f) { int i = 0, r = 0, c; char* str = NULL;
  while (isspace(c = fgetc(f)));
  if (c == EOF) return NULL;
  do {
    if (r == 0) {
      str = realloc(str, i + STEP);
      r = STEP;
    }
    --r;
    str[i] = c;
    ++i;
  } while ((c = fgetc(f)) != EOF && !isspace(c));
  str = realloc(str, i + 1);
  str[i] = 0;
  return str;
}

tree* tree_init(FILE* f) {
  tree* tmp_h;
  tmp_h = malloc(sizeof(tree));
  tmp_h->left = tmp_h->right = NULL;
  tmp_h->word = read_word(f);
  tmp_h->count = 1;
  return tmp_h;
}

tree* find(char* str, tree* head, tree** parent) {
  int cmp_val;
  while (head) {
    *parent = head;
    if ((cmp_val = strcmp(str, head->word)) > 0)
      head = head->right;
    else if (cmp_val < 0)
      head = head->left;
    else return head;
  }
  return NULL;
}

void add_to_tree(char* str, tree* parent) {
  if (strcmp(str, parent->word) > 0) {
    parent->right = malloc(sizeof(tree));
    parent = parent->right;
  }
  else {
    parent->left = malloc(sizeof(tree));
    parent = parent->left;
  }
  parent->count = 1;
  parent->word = str;
  parent->left = parent->right = NULL;
  return;
}

void put_tree_in_arr(tree** arr, int* index_ptr, tree* head) {
  if (head) {
    arr[*index_ptr] = head;
    ++(*index_ptr);
    put_tree_in_arr(arr, index_ptr, head->left);
    put_tree_in_arr(arr, index_ptr, head->right);
  }
  return;
}

int cmp(tree** frst, tree** scnd) {
  return (*scnd)->count - (*frst)->count;
}

void final_output(tree** arr, tree** arr_end, int count_all, FILE* f) {
  for (; arr < arr_end; ++arr)
    fprintf(f, "%s %d %f\n", (*arr)->word, (*arr)->count, (float)(*arr)->count/count_all);
  return;
}

void free_tree(tree* head) {
  if (head->left)
    free_tree(head->left);
  if (head->right)
    free_tree(head->right);
  free(head->word);
  free(head);
  return;
}

int main(int argc, char** argv) {
  char *f_in_name = NULL, *f_out_name = NULL;
  keys_processing(argc, argv, &f_in_name, &f_out_name);
  FILE *f_in = stdin, *f_out = stdout;
  open_files(f_in_name, f_out_name, &f_in, &f_out);
  tree *cur, *head, *parent;
  char* str;
  head = tree_init(f_in);
  int count_all = 1, nodes_count = 1;
  while (str = read_word(f_in)) {
    if (!(cur = find(str, head, &parent))) {
      add_to_tree(str, parent);
      ++nodes_count;
    }
    else {
      ++cur->count;
      free(str);
    }
    ++count_all;
  }
  tree** arr = malloc(nodes_count * sizeof(tree*));
  int index = 0;
  put_tree_in_arr(arr, &index, head);
  qsort(arr, nodes_count, sizeof(tree*), (int(*)(const void*, const void*))cmp);
  final_output(arr, arr + nodes_count, count_all, f_out);
  free_tree(head);
  free(arr);
  fclose(f_in);
  fclose(f_out);
  return 0;
}
