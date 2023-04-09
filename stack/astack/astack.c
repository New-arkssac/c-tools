#include "astack.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define __new_stack(cap) malloc(cap)
#define __index(stack, index) (stack->data + index * stack->type_size)
#define die(n, msg)                                                            \
  {                                                                            \
    perror(msg);                                                               \
    exit(n);                                                                   \
  }

astack new_astack(long cap, long type_size) {
  if (cap < 0)
    cap = 0;

  astack stack = {
      .data = __new_stack(cap * type_size),
      .top = 0,
      .cap = cap,
      .type_size = type_size,
      .closed = 0,
  };

  return stack;
}

int astack_peek(astack *stack, void *data) {
  if (astack_is_empty(stack))
    return ERR_ASTACK_IS_EMPTY;

  memcpy(data, __index(stack, (stack->top - 1)), stack->type_size);
  return 0;
}

int astack_pop(astack *stack, void *data) {
  if (astack_is_empty(stack))
    return ERR_ASTACK_IS_EMPTY;

  memcpy(data, __index(stack, --(stack->top)), stack->type_size);
  return 0;
}

int astack_push(astack *stack, const void *data) {
  if (!astack_valid(stack))
    return ERR_ASTACK_IS_NULL;

  if (stack->top >= stack->cap)
    return ERR_ASTACK_IS_FULL;

  memcpy(__index(stack, stack->top++), data, stack->type_size);
  return 0;
}

void astack_close(astack *stack) {
  if (!astack_valid(stack))
    die(1, "astack is a null pointer");

  if (stack->closed)
    die(1, "closes an already closed astack");

  free(stack->data);
  stack->data = NULL;
  stack->closed = 1;
  stack->cap = stack->top = stack->type_size = 0;
}
