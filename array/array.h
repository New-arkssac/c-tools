#ifndef __BUFFER
#define __BUFFER
#include <stdlib.h>

enum {
  ERR_ZERO_LENGHT = 10,
  ERR_INVALID_INDEX,
  ERR_ALLOCATION_FAILED,
  ERR_NULL_POINTER,
  ERR_TYPE_SIZE_NOT_EQUAL,
};

// WARNING: This is a buffer that can store any data type and will automatically
// expand itself. Do not modify any member data in the structure directly.
// please use the buffer's functions to access and modify the data.
// do not attempt to modify the type size property of a created buffer, as this
// may cause unknown issues. It is wise to create a new buffer with the desired
// data type if necessary.
typedef struct array_buffer array_buffer;
struct array_buffer {
  // Store data of any type.
  void *buffer;
  // Bufferer lenght
  long lenght;
  // Bufferer capacity
  long cap;
  // Data type size
  long type_size;
};

array_buffer new_array(long cap, long type_size);
array_buffer array_slice(array_buffer *src, long start, long end);
int array_delete(array_buffer *src, array_buffer *news, long start, long end);
int array_insert(array_buffer *src, void *const data, array_buffer *news,
                 long start, long end);
int array_imore(array_buffer *src, array_buffer *data, array_buffer *news,
                long start, long end);
int array_append(array_buffer *src, array_buffer *data);
int array_add(array_buffer *src, void *const data);
int array_get(array_buffer *src, void *const data, long index);
int array_modify(array_buffer *src, void *const data, long index);
int array_clone(array_buffer *src, array_buffer *news);
void *array_ptr(array_buffer *src);

#define array_valid(array) (array != NULL && (array)->buffer != NULL)
// Release some bufferer.
#define array_release(...)                                                     \
  do {                                                                         \
    array_buffer *f[] = {__VA_ARGS__};                                         \
    for (size_t i = 0; i < sizeof(f) / sizeof(f[0]); i++) {                    \
      free(f[i]->buffer);                                                      \
      f[i]->buffer = NULL;                                                     \
      f[i]->lenght = 0;                                                        \
      f[i]->cap = 0;                                                           \
      f[i]->type_size = 0;                                                     \
    }                                                                          \
  } while (0)

// Print all elements in the buffer.
#define array_print(array, sep, type)                                          \
  do {                                                                         \
    type elem;                                                                 \
    int err;                                                                   \
    printf("[");                                                               \
    for (int i = 0; i < array.lenght; i++) {                                   \
      if ((err = array_get(&array, &elem, i)) != 0)                            \
        break;                                                                 \
      printf(sep, elem);                                                       \
    }                                                                          \
    printf("]\n");                                                             \
  } while (0)

#endif /* ifndef __BUFFER */
