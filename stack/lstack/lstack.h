#ifndef __LINKED_STACK
#define __LINKED_STACK

enum {
  ERR_NULL_POINTER = 10,
  ERR_LINKED_IS_EMPTY,
};

typedef struct lstack lstack;
typedef struct list list;

// stack of unlimited length
struct lstack {
  list *head;
  long cap;
  long type_size;
  int closed : 1;
};

lstack new_lstack(long type_size);
int lstack_peek(lstack *stack, void *data);
int lstack_pop(lstack *stack, void *data);
int lstack_push(lstack *stack, const void *data);
void lstack_close(lstack *stack);

#define lstack_valid(stack) (stack != NULL)
#define lstack_is_empty(stack) (!lstack_valid(stack) || stack->cap == 0)

#define lstack_batch_close(...)                                                \
  do {                                                                         \
    lstack *p[] = {__VA_ARGS__};                                               \
    for (int i = 0; i < sizeof(p) / sizeof(p[0]); i++)                         \
      lstack_close(p[i]);                                                      \
  } while (0)

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
#endif /* ifndef __lstack */
