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

#define ut_da_pop(da, index)                           \
  do {                                                 \
    assert((da)->count != 0);                          \
    for (size_t k = (index); k < (da)->count - 1; k++) \
      (da)->items[k] = (da)->items[k + 1];             \
    (da)->count--;                                     \
  } while(0)

#define ut_da_free(da) (free((da)->items))

#endif // UT_H

#ifndef UTDEF
/*
 * Goes before declarations and definitions of the nob functions. Useful to `#define UTDEF static inline`
 * if your source code is a single file and you want the compiler to remove unused functions.
*/
#define UTDEF

#endif /* UTDEF */

/*
 * Also include implementations.
 * Almost always declared by user.
*/
#ifdef UT_IMPLEMENTATION

#endif // UT_IMPLEMENTATION
