#include "aqueue.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define __new_aqueue(cap) malloc(cap)
#define __queue_index(index, cap) ((index) % (cap))
#define __data_index(queue, index)                                             \
  ((queue->data) + (index) * (queue->type_size))

#define die(n, msg)                                                            \
  {                                                                            \
    perror(msg);                                                               \
    exit(n);                                                                   \
  }

aqueue new_aqueue(long cap, long type_size) {
  if (cap < 0)
    cap = 0;

  aqueue queue = {
      .data = __new_aqueue(cap * type_size),
      .head = 0,
      .tail = 0,
      .lenght = 0,
      .cap = cap,
      .type_size = type_size,
      .closed = 0,
  };

  return queue;
}

int aqueue_push(aqueue *queue, const void *data) {
  if (!aqueue_valid(queue))
    return ERR_AQUEUE_IS_NULL;

  if (aqueue_is_full(queue))
    return ERR_AQUEUE_IS_FULL;

  memcpy(__data_index(queue, __queue_index(queue->tail++, queue->cap)), data,
         queue->type_size);
  queue->lenght++;
  return 0;
}

int aqueue_pop(aqueue *queue, void *data) {
  if (aqueue_is_empty(queue))
    return ERR_AQUEUE_IS_EMPTY;

  memcpy(data, __data_index(queue, __queue_index(queue->head++, queue->cap)),
         queue->type_size);
  queue->lenght--;
  return 0;
}

int aqueue_peek(aqueue *queue, void *data) {
  if (aqueue_is_empty(queue))
    return ERR_AQUEUE_IS_EMPTY;

  memcpy(data, __data_index(queue, queue->head), queue->type_size);
  return 0;
}

void aqueue_close(aqueue *queue) {
  if (!aqueue_valid(queue))
    die(1, "aqueue is a null pointer") return;

  if (queue->closed)
    die(1, "closes an already closed aqueue") return;

  free(queue->data);
  queue->data = NULL;
  queue->closed = 1;
  queue->type_size = queue->lenght = queue->cap = queue->head = queue->tail = 0;
}
