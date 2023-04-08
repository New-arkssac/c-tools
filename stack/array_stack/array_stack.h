#ifndef __ARRAY_STACK
#define __ARRAY_STACK

typedef struct array_stack array_stack;

enum {
  ERR_STACK_IS_EMPTY = 10,
  ERR_STACK_IS_FULL,
};

struct array_stack {
  void *data;
  long top;
  long cap;
  long type_size;
};

array_stack new_astack(long cap, long type_size);
int astack_is_empty(array_stack *stack);
int astack_peek(array_stack *stack, void *data);
int astack_pop(array_stack *stack, void *data);
int astack_push(array_stack *stack, const void *data);
#define astack_release(...)                                                    \
  do {                                                                         \
    void *p[] = {__VA_ARGS__};                                                 \
    for (int i = 0; i < sizeof(p) / sizeof(p[0]); i++) {                       \
      free(((array_stack *)p[i])->data);                                       \
      p[i] = NULL;                                                             \
    }                                                                          \
  } while (0);

#define astack_print(stack, sep, type)                                         \
  {                                                                            \
    printf("[");                                                               \
    for (int i = stack.cap - 1; i >= 0; i--)                                   \
      printf(sep, *(type *)(stack.data + i * stack.type_size));                \
    printf("]\n");                                                             \
  }

#endif /* ifndef __ARRAY_STACK */
