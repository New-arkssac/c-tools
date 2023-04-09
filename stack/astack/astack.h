#ifndef __astack
#define __astack

typedef struct astack astack;

enum {
  ERR_ASTACK_IS_EMPTY = 10,
  ERR_ASTACK_IS_FULL,
  ERR_ASTACK_IS_NULL,
};

struct astack {
  void *data;
  long top, cap;
  long type_size;
  int closed : 1;
};

astack new_astack(long cap, long type_size);
int astack_is_empty(astack *stack);
int astack_peek(astack *stack, void *data);
int astack_pop(astack *stack, void *data);
int astack_push(astack *stack, const void *data);
void astack_close(astack *stack);

#define astack_valid(stack) (stack != NULL && (stack)->data != NULL)
#define astack_is_empty(stack) (!astack_valid(stack) || (stack)->top == 0)
#define astack_batch_closes(...)                                               \
  do {                                                                         \
    astack *p[] = {__VA_ARGS__};                                               \
    for (int i = 0; i < sizeof(p) / sizeof(p[0]); i++)                         \
      astack_close(p[i]);                                                      \
  } while (0)

#define astack_print(stack, sep, type)                                         \
  {                                                                            \
    printf("[");                                                               \
    for (int i = stack.cap - 1; i >= 0; i--)                                   \
      printf(sep, *(type *)(stack.data + i * stack.type_size));                \
    printf("]\n");                                                             \
  }

#endif /* ifndef __astack */
