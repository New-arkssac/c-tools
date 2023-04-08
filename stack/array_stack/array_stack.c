#include "array_stack.h"
#include <memory.h>
#include <stdlib.h>

#define __new_stack(cap) malloc(cap)
#define __index(stack, index) (stack->data + index * stack->type_size)

array_stack new_astack(long cap, long type_size) {
  if (cap < 0)
    cap = 0;

  array_stack stack = {
      .data = __new_stack(cap * type_size),
      .top = 0,
      .cap = cap,
      .type_size = type_size,
  };

  return stack;
}

int astack_is_empty(array_stack *stack) {
  return stack->top == 0 || stack == NULL;
}

int astack_peek(array_stack *stack, void *data) {
  if (astack_is_empty(stack))
    return ERR_STACK_IS_EMPTY;

  memcpy(data, __index(stack, (stack->top - 1)), stack->type_size);
  return 0;
}

int astack_pop(array_stack *stack, void *data) {
  if (astack_is_empty(stack))
    return ERR_STACK_IS_EMPTY;

  memcpy(data, __index(stack, --(stack->top)), stack->type_size);
  return 0;
}

int astack_push(array_stack *stack, const void *data) {
  if (stack->top >= stack->cap)
    return ERR_STACK_IS_FULL;

  memcpy(__index(stack, stack->top++), data, stack->type_size);
  return 0;
}
