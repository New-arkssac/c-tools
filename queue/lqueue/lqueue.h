#ifndef __LQUEUE
#define __LQUEUE

enum {
  ERR_NULL_POINTER = 10,
  ERR_LQUEUE_IS_EMPTY,
};

typedef struct queue_list queue_list;
typedef struct lqueue lqueue;

struct lqueue {
  queue_list *head;
  queue_list *tail;
  long lenght;
  long type_size;
  int closed : 1;
};

lqueue new_lqueue(long type_size);
int lqueue_pop(lqueue *queue, void *data);
int lqueue_peek(lqueue *queue, void *data);
int lqueue_push(lqueue *queue, const void *data);
void lqueue_close(lqueue *queue);

#define lqueue_valid(queue) (queue != NULL)
#define lqueue_is_empty(queue) (!lqueue_valid(queue) || (queue)->lenght == 0)

#define lqueue_batch_close(...)                                                \
  do {                                                                         \
    lqueue *p[] = {__VA_ARGS__};                                               \
    for (int i = 0; i < sizeof(p) / sizeof(p[0]); i++)                         \
      lqueue_close(p[i]);                                                      \
  } while (0)

#endif /* ifndef __LQUEUE */
