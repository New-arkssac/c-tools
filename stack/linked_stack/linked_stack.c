#include "linked_stack.h"
#include <memory.h>
#include <stdlib.h>

#define __new_list(cap) malloc(cap)
#define __new_data(cap) malloc(cap)
#define __new_node(stack)                                                      \
  ({                                                                           \
    node = __new_list(sizeof(linked_list));                                    \
    if (node == NULL)                                                          \
      return ERR_NULL_POINTER;                                                 \
                                                                               \
    node->data = __new_data(stack->type_size);                                 \
                                                                               \
    if (node->data == NULL) {                                                  \
      free(node);                                                              \
      return ERR_NULL_POINTER;                                                 \
    }                                                                          \
                                                                               \
    node->next = NULL;                                                         \
    node;                                                                      \
  })

linked_stack new_lstack(long type_size) {
  linked_stack stack = {
      .head = NULL,
      .cap = 0,
      .type_size = type_size,
  };

  return stack;
}

int lstack_peek(linked_stack *stack, void *data) {
  if (lstack_is_empty(stack))
    return ERR_LINKED_IS_EMPTY;

  memcpy(data, stack->head->data, stack->type_size);
  return 0;
}

int lstack_is_empty(linked_stack *stack) {
  return stack->cap == 0 || stack->head == NULL;
}

int lstack_pop(linked_stack *stack, void *data) {
  if (lstack_is_empty(stack))
    return ERR_LINKED_IS_EMPTY;

  memcpy(data, stack->head->data, stack->type_size);
  linked_list *node = stack->head;
  stack->head = node->next;
  free(node->data);
  free(node);
  stack->cap--;
  return 0;
}

int lstack_push(linked_stack *stack, const void *data) {
  linked_list *node = __new_node(stack);

  if (stack->head == NULL) {
    stack->head = node;
  } else {
    node->next = stack->head;
    stack->head = node;
  }

  memcpy(node->data, data, stack->type_size);
  stack->cap++;
  return 0;
}
