#ifndef __LINKED_STACK
#define __LINKED_STACK

enum {
  ERR_NULL_POINTER = 10,
  ERR_LINKED_IS_EMPTY,
};

typedef struct linked_stack linked_stack;
typedef struct linked_list linked_list;

struct linked_list {
  void *data;
  linked_list *next;
};

// stack of unlimited length
struct linked_stack {
  linked_list *head;
  long cap;
  long type_size;
};

linked_stack new_lstack(long type_size);
int lstack_is_empty(linked_stack *stack);
int lstack_peek(linked_stack *stack, void *data);
int lstack_pop(linked_stack *stack, void *data);
int lstack_push(linked_stack *stack, const void *data);

#define lstack_release(...)                                                    \
  do {                                                                         \
    linked_stack *p[] = {__VA_ARGS__};                                         \
    linked_list *node, *curr;                                                  \
    for (int i = 0; i < sizeof(p) / sizeof(p[0]); i++) {                       \
      curr = p[i]->head;                                                       \
      while (node != NULL) {                                                   \
        curr = node;                                                           \
        node = curr->next;                                                     \
        free(curr->data);                                                      \
        free(curr);                                                            \
      }                                                                        \
      p->head = NULL;                                                          \
      p->cap = 0;                                                              \
      p->type_size = 0;                                                        \
    }                                                                          \
  } while (0);

#define lstack_print(stack, sep, type)                                         \
  do {                                                                         \
    printf("[");                                                               \
    linked_list *p = stack.head;                                               \
    while (p != NULL) {                                                        \
      printf(sep, *(type *)p->data);                                           \
      p = p->next;                                                             \
    }                                                                          \
    printf("]\n");                                                             \
  } while (0)
#endif /* ifndef __LINKED_STACK */
