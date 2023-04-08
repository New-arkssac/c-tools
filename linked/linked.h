#ifndef __linked_BUFFER
#define __linked_BUFFER

enum {
  ERR_NULL_POINTER = 1,
  ERR_LINKED_IS_ZERO,
  ERR_INVALID_INDEX,
  ERR_LINKED_IS_NULL,
  ERR_LINKED_TYPE_SIZE,
};

typedef struct linked_buffer linked_buffer;
typedef struct linked_list linked_list;

struct linked_buffer {
  linked_list *head;
  linked_list *tail;
  long lenght;
  long type_size;
};

linked_buffer new_linked(long type_size);
int linked_get(linked_buffer *buffer, void *data, int index);
int linked_first_add(linked_buffer *buffer, void *data);
int linked_after_add(linked_buffer *buffer, void *data);
int linked_append(linked_buffer *buffer, linked_buffer *add, int index);
int linked_modify(linked_buffer *buffer, void *data, int index);
int linked_insert(linked_buffer *buffer, void *data, int index);
int linked_delete(linked_buffer *buffer, int index);
int linked_check(linked_buffer *buffer);
void linked_release(int n, ...);

#define linked_print(buffer, sep, type)                                        \
  {                                                                            \
    printf("[");                                                               \
    type elem;                                                                 \
    for (int i = 0; i < buffer.lenght; i++) {                                  \
      linked_get(&buffer, &elem, i);                                           \
      printf(sep, elem);                                                       \
    }                                                                          \
    printf("]\n");                                                             \
  }
#endif /* ifndef __linked_BUFFER */
