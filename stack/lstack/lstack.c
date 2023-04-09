#include "lstack.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define __new_list(cap) malloc(cap)
#define __new_data(cap) malloc(cap)
#define __new_node(stack)                                                      \
  ({                                                                           \
    node = __new_list(sizeof(list));                                           \
    if (node == NULL)                                                          \
      return ERR_NULL_POINTER;                                                 \
                                                                               \
    node->data = __new_data((stack)->type_size);                               \
                                                                               \
    if (node->data == NULL) {                                                  \
      free(node);                                                              \
      return ERR_NULL_POINTER;                                                 \
    }                                                                          \
                                                                               \
    node->next = NULL;                                                         \
    node;                                                                      \
  })

#define die(n, msg)                                                            \
  {                                                                            \
    perror(msg);                                                               \
    exit(n);                                                                   \
  }

struct list {
  void *data;
  list *next;
};

lstack new_lstack(long type_size) {
  lstack stack = {
      .head = NULL,
      .cap = 0,
      .type_size = type_size,
      .closed = 0,
  };

  return stack;
}

int lstack_peek(lstack *stack, void *data) {
  if (lstack_is_empty(stack))
    return ERR_LINKED_IS_EMPTY;

  memcpy(data, stack->head->data, stack->type_size);
  return 0;
}

int lstack_pop(lstack *stack, void *data) {
  if (lstack_is_empty(stack))
    return ERR_LINKED_IS_EMPTY;

  memcpy(data, stack->head->data, stack->type_size);
  list *node = stack->head;
  stack->head = node->next;
  free(node->data);
  free(node);
  stack->cap--;
  return 0;
}

int lstack_push(lstack *stack, const void *data) {
  if (!lstack_valid(stack))
    return ERR_NULL_POINTER;

  list *node = __new_node(stack);

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

void lstack_close(lstack *stack) {
  if (!lstack_valid(stack))
    die(1, "lstack is a null pointer");

  if (stack->closed)
    die(1, "closes an already closed lstack");

  list *node, *curr = stack->head;
  while (curr != NULL) {
    node = curr;
    curr = node->next;
    free(node->data);
    free(node);
  }
  stack->head = NULL;
  stack->cap = 0;
  stack->type_size = 0;
}
