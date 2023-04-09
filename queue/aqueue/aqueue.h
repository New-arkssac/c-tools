#ifndef __AQUEUE
#define __AQUEUE
enum {
  ERR_AQUEUE_IS_FULL = 10,
  ERR_AQUEUE_IS_EMPTY,
  ERR_AQUEUE_IS_NULL,
  ERR_AQUEUE_CLOSES_AN_ALREADY_CLOSED,
};

typedef struct aqueue aqueue;

struct aqueue {
  void *data;
  long head, tail;
  long lenght, cap;
  long type_size;
  int closed : 1;
};

aqueue new_aqueue(long cap, long type_size);
int aqueue_push(aqueue *queue, const void *data);
int aqueue_pop(aqueue *queue, void *data);
int aqueue_peek(aqueue *queue, void *data);
void aqueue_close(aqueue *queue);
#define aqueue_batch_closes(...)                                               \
  do {                                                                         \
    aqueue *p[] = {__VA_ARGS__};                                               \
    for (int i = 0; i < sizeof(p) / sizeof(p[0]); i++)                         \
      aqueue_close(p[i]);                                                      \
  } while (0)

#define aqueue_valid(queue) ((queue) != NULL && (queue)->data != NULL)
#define aqueue_is_full(queue) ((queue)->lenght == (queue->cap))
#define aqueue_is_empty(queue) (!aqueue_valid(queue) && (queue)->lenght == 0)

#endif /* ifndef __AQUEUE */
