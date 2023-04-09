#ifndef __linked_BUFFER
#define __linked_BUFFER

enum {
  ERR_NULL_POINTER = 10,
  ERR_LINKED_IS_ZERO,
  ERR_INVALID_INDEX,
  ERR_LINKED_IS_NULL,
  ERR_LINKED_TYPE_SIZE,
};

typedef struct linked_buffer linked_buffer;
typedef struct linked_list linked_list;

struct linked_list {
  void *data;
  linked_list *prev;
  linked_list *next;
};

struct linked_buffer {
  linked_list *head;
  linked_list *tail;
  long lenght;
  long type_size;
  int closed : 1;
};

linked_buffer new_linked(long type_size);
int linked_get(linked_buffer *buffer, void *data, int index);
int linked_first_add(linked_buffer *buffer, void *data);
int linked_after_add(linked_buffer *buffer, void *data);
int linked_append(linked_buffer *buffer, linked_buffer *add, int index);
int linked_modify(linked_buffer *buffer, void *data, int index);
int linked_insert(linked_buffer *buffer, void *data, int index);
int linked_delete(linked_buffer *buffer, int index);
void linked_close(linked_buffer *buffer);

#define linked_valid(buffer) (buffer != NULL && (buffer)->head != NULL)
#define linked_is_empty(buffer) (!linked_valid(buffer) || (buffer->lenght) == 0)

#define linked_batch_closes(...)                                               \
  do {                                                                         \
    linked_buffer *f[] = {__VA_ARGS__};                                        \
    for (int i = 0; i < sizeof(f) / sizeof(f[0]); i++)                         \
      linked_close(f[i]);                                                      \
  } while (0)

#define linked_print(buffer, sep, type)                                        \
  do {                                                                         \
    printf("[");                                                               \
    type elem;                                                                 \
    int err;                                                                   \
    for (int i = 0; i < buffer.lenght; i++) {                                  \
      if ((err = linked_get(&buffer, &elem, i) != 0))                          \
        break;                                                                 \
      printf(sep, elem);                                                       \
    }                                                                          \
    printf("]\n");                                                             \
  } while (0)
#endif /* ifndef __linked_BUFFER */
