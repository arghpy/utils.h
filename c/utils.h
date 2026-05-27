/*
 * This will always be included.
 * This is used in order to not include it multiple times.
*/
#ifndef UT_H
#define UT_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define SWAP(T, a, b) \
  do {                \
    T tmp = a;        \
    a = b;            \
    b = tmp;          \
  } while(0)          \

#define ARRAY_LEN(arr) (sizeof((arr))/sizeof((arr)[0]))

#define REVERSE_ARRAY(T, arr_ptr)              \
  do {                                         \
    if ((arr_ptr) != NULL) {                   \
      size_t i = ARRAY_LEN((arr_ptr)) - 1;     \
      size_t j = 0;                            \
      while (i > j)                            \
      {                                        \
        SWAP(T, (arr_ptr)[i], (arr_ptr)[j]); \
        i--;                                   \
        j++;                                   \
      }                                        \
    }                                          \
  } while(0)                                   \

#define ut_da_declare(type)  \
  struct {                   \
    type *items;             \
    size_t count;            \
    size_t capacity;         \
  }

/*
 * The initial capacity of the dynamic array.
 * Can be changed
*/
#ifndef UT_INIT_CAP
#define UT_INIT_CAP 256
#endif

#define ut_da_push(da, item)                                                         \
  do {                                                                               \
    if ((da)->items == NULL) {                                                       \
      (da)->capacity = UT_INIT_CAP;                                                  \
      (da)->items = calloc((da)->capacity, sizeof((da)->items[0]));                  \
    }                                                                                \
    if ((da)->count >= (da)->capacity) {                                             \
      (da)->capacity *= 2;                                                           \
      (da)->items = realloc((da)->items, sizeof((da)->items[0]) * (da)->capacity);   \
    }                                                                                \
    assert((da)->items != NULL);                                                     \
    (da)->items[(da)->count] = (item);                                                 \
    (da)->count += 1;                                                                \
  } while(0)

#define ut_da_reset(da)                                           \
  do {                                                            \
    if ((da)->items != NULL) ut_da_free((da));                    \
    (da)->capacity = UT_INIT_CAP;                                 \
    (da)->items = calloc((da)->capacity, sizeof((da)->items[0])); \
    (da)->count = 0;                                              \
  } while(0)

#define ut_da_copy(dest, src)                                                      \
  do {                                                                             \
    if ((dest)->items != NULL) ut_da_free((dest));                                 \
    (dest)->count = (src)->count;                                                  \
    (dest)->capacity = (src)->capacity;                                            \
    (dest)->items = calloc((dest)->capacity, sizeof((dest)->items[0]));            \
    memcpy((dest)->items, (src)->items, sizeof((dest)->items[0]) * (dest)->count); \
  } while(0)                                                                       \

#define ut_da_remove(da, index)                        \
  do {                                                 \
    assert((da)->count != 0);                          \
    for (size_t k = (index); k < (da)->count - 1; k++) \
      (da)->items[k] = (da)->items[k + 1];             \
    (da)->count--;                                     \
  } while(0)

#define ut_da_pop(da)         \
  do {                        \
    assert((da)->count != 0); \
    (da)->count--;            \
  } while(0)

#define ut_da_free(da) (free((da)->items))


// Linked lists
#define ut_ll_declare(NodeName, T) \
  typedef struct NodeName {        \
    struct NodeName* prev;         \
    T value;                       \
    struct NodeName* next;         \
  } NodeName


#define ut_ll_free(NodeName, ll_head_ptr) \
  do {                                \
    NodeName *c = (ll_head_ptr);          \
    while (c != NULL) {               \
      NodeName *tmp = c->next;        \
      free(c);                        \
      c = tmp;                        \
    }                                 \
    (ll_head_ptr) = NULL;                 \
  } while(0)                          \

#define ut_ll_push(NodeName, ll_head_ptr, val, ll_tail_ptr) \
  do {                                                      \
    NodeName *t = malloc(sizeof(NodeName));                 \
    if (!t) fprintf(stderr, "Could not request memory.\n"); \
    else {                                                  \
      t->prev  = NULL;                                      \
      t->value = (val);                                     \
      t->next  = NULL;                                      \
      if ((ll_head_ptr) == NULL) {                          \
        (ll_head_ptr) = t;                                  \
        (ll_tail_ptr) = t;                                  \
      } else {                                              \
        NodeName *c = (ll_head_ptr);                        \
        while (c->next != NULL) {                           \
          c = c->next;                                      \
        }                                                   \
        c->next = t;                                        \
        t->prev = c;                                        \
                                                            \
        (ll_tail_ptr) = t;                                  \
      }                                                     \
    }                                                       \
  } while(0)                                                \

#endif // UT_H

#ifndef UTDEF
/*
 * Goes before declarations and definitions of the ut functions. Useful to `#define UTDEF static inline`
 * if your source code is a single file and you want the compiler to remove unused functions.
*/
#define UTDEF

#endif /* UTDEF */

#include <stdio.h>

UTDEF void ut_strrev(char *str);
UTDEF char* read_entire_file(const char* filename);

#ifdef UT_IMPLEMENTATION

// Reverse a normal string
UTDEF void ut_strrev(char *str)
{
  if (str && *str) {
    int i = strlen(str) - 1, j = 0;
    char ch;

    while (i > j)
    {
      ch = str[i];
      str[i] = str[j];
      str[j] = ch;
      i--;
      j++;
    }
  }
}

char* read_entire_file(const char* filename)
{
  FILE *f = fopen(filename, "r");
  if (!f) {
    // TODO: maybe make use of errno?
    fprintf(stderr, "Could not open file: %s\n", filename);
    exit(EXIT_FAILURE);
  }

  // Get file size
  fseek(f, 0, SEEK_END);
  long file_size = ftell(f);
  if (file_size == -1) {
    fprintf(stderr, "Could not determine file size of %s\n", filename);
    exit(EXIT_FAILURE);
  }
  rewind(f);

  // Read contents
  char* data = calloc(file_size + 1, sizeof(char));
  if (!data) {
    fprintf(stderr, "Could not allocate enough memory to read file: %s\n", filename);
    exit(EXIT_FAILURE);
  }

  size_t ret = fread(data, sizeof(*data), file_size, f);
  if ((long)ret != file_size) {
    fprintf(stderr, "Could not read all file: %s\n", filename);
    fprintf(stderr, "Expected: %ld\n", file_size);
    fprintf(stderr, "Read    : %zu\n", ret);
    free(data);
    fclose(f);
    exit(EXIT_FAILURE);
  }

  fclose(f);
  return data;
}

#endif // UT_IMPLEMENTATION
