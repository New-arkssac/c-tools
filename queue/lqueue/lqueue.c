#include "lqueue.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define __new_list(cap) malloc(cap)
#define __new_data(cap) malloc(cap)
#define __new_node(queue)                                                      \
  ({                                                                           \
    node = __new_list(sizeof(queue_list));                                     \
    if (node == NULL)                                                          \
      return ERR_NULL_POINTER;                                                 \
                                                                               \
    node->data = __new_data((queue)->type_size);                               \
                                                                               \
    if (node->data == NULL) {                                                  \
      free(node);                                                              \
      return ERR_NULL_POINTER;                                                 \
    }                                                                          \
    node->prev = NULL;                                                         \
    node->next = NULL;                                                         \
    node;                                                                      \
  })

#define die(n, msg)                                                            \
  {                                                                            \
    perror(msg);                                                               \
    exit(n);                                                                   \
  }

struct queue_list {
  void *data;
  queue_list *prev;
  queue_list *next;
};

lqueue new_lqueue(long type_size) {
  lqueue queue = {
      .head = NULL,
      .tail = NULL,
      .lenght = 0,
      .type_size = type_size,
      .closed = 0,
  };

  return queue;
}

int lqueue_push(lqueue *queue, const void *data) {
  if (!lqueue_valid(queue))
    return ERR_NULL_POINTER;

  queue_list *node = __new_node(queue);
  if (queue->head == NULL) {
    queue->head = node;
    queue->tail = node;
  } else {
    queue->head->prev = node;
    node->next = queue->head;
    queue->head = node;
  }

  memcpy(node->data, data, queue->type_size);
  queue->lenght++;
  return 0;
}

int lqueue_pop(lqueue *queue, void *data) {
  if (lqueue_is_empty(queue))
    return ERR_LQUEUE_IS_EMPTY;

  queue_list *prev = queue->tail->prev;
  memcpy(data, queue->tail->data, queue->type_size);
  free(queue->tail->data);
  free(queue->tail);
  queue->tail = prev;
  queue->lenght--;
  return 0;
}

int lqueue_peek(lqueue *queue, void *data) {
  if (lqueue_is_empty(queue))
    return ERR_LQUEUE_IS_EMPTY;

  memcpy(data, queue->tail->data, queue->type_size);
  return 0;
}

void lqueue_close(lqueue *queue) {
  if (!lqueue_valid(queue))
    die(1, "lqueue is a null pointer");

  if (queue->closed)
    die(1, "closes an already closed lqueue");

  queue_list *node, *curr = queue->head;
  while (curr != NULL) {
    node = curr;
    curr = node->next;
    free(node->data);
    free(node);
  }

  queue->head = queue->tail = NULL;
  queue->closed = 1;
  queue->lenght = queue->type_size = 0;
}
