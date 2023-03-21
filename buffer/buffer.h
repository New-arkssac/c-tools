#ifndef __BUFFER
#define __BUFFER

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
typedef struct Buffer Buffer;
struct Buffer {
  // Store data of any type.
  void *buffer;
  // Bufferer lenght
  long lenght;
  // Bufferer capacity
  long cap;
  // Data type size
  long type_size;
};

Buffer new_buffer(long cap, long type_size);
Buffer buffer_slice(Buffer *src, long start, long end);
int buffer_delete(Buffer *src, Buffer *news, long start, long end);
int buffer_insert(Buffer *src, void *const data, Buffer *news, long start,
                  long end);
int buffer_imore(Buffer *src, Buffer *data, Buffer *news, long start, long end);
int buffer_append(Buffer *src, Buffer *data);
int buffer_add(Buffer *src, void *const data);
int buffer_get(Buffer *src, void *const data, long index);
int buffer_clone(Buffer *src, Buffer *news);
int buffer_null(Buffer *src);
void *buffer_ptr(Buffer *src);
void buffer_release(int num, ...);

// Print all elements in the buffer.
#define buffer_print(buf, sep, type)                                           \
  {                                                                            \
    type elem;                                                                 \
    int err;                                                                   \
    printf("[");                                                               \
    for (int i = 0; i < buf.lenght; i++) {                                     \
      if ((err = buffer_get(&buf, &elem, i)) != 0) {                           \
        return err;                                                            \
      }                                                                        \
      printf(sep, elem);                                                       \
    }                                                                          \
    printf("]\n");                                                             \
  }

#endif /* ifndef __BUFFER */
