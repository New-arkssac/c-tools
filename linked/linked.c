#include "linked.h"
#include <memory.h>
#include <stdarg.h>
#include <stdlib.h>

#define new_data(cap) malloc(cap);
#define new_list(cap) malloc(cap);
#define check_buffer(buffer)                                                   \
  (linked_check(buffer) ? ERR_LINKED_IS_NULL                                   \
                        : (buffer->lenght == 0         ? ERR_LINKED_IS_ZERO    \
                           : (index >= buffer->lenght) ? ERR_INVALID_INDEX     \
                                                       : 0))
#define new_node()                                                             \
  ({                                                                           \
    node = new_list(sizeof(linked_buffer));                                    \
    if (node == NULL)                                                          \
      return ERR_NULL_POINTER;                                                 \
                                                                               \
    node->data = new_data(buffer->type_size);                                  \
                                                                               \
    if (node->data == NULL) {                                                  \
      free(node);                                                              \
      return ERR_NULL_POINTER;                                                 \
    }                                                                          \
                                                                               \
    node->prev = NULL;                                                         \
    node->next = NULL;                                                         \
    node;                                                                      \
  })

#define node_free(node)                                                        \
  {                                                                            \
    free(node->data);                                                          \
    free(node);                                                                \
  }

struct linked_list {
  void *data;
  linked_list *prev;
  linked_list *next;
};

linked_list *get_node(linked_buffer *buffer, int index);
linked_list *get_node(linked_buffer *buffer, int index) {
  if (index == 0)
    return buffer->head;

  if (index == buffer->lenght - 1)
    return buffer->tail;

  linked_list *p = buffer->head;
  while (index--)
    p = p->next;

  return p;
}

linked_buffer new_linked(long type_size) {
  linked_buffer head = {
      .head = NULL,
      .tail = NULL,
      .lenght = 0,
      .type_size = type_size,
  };

  return head;
}

int linked_first_add(linked_buffer *buffer, void *data) {
  if (linked_check(buffer))
    return ERR_LINKED_IS_NULL;

  linked_list *p = buffer->head;
  linked_list *node = new_node();

  if (p == NULL) {
    buffer->head = node;
    buffer->tail = node;
  } else {
    buffer->head->prev = node;
    node->next = buffer->head;
    buffer->head = node;
  }

  memcpy(node->data, data, buffer->type_size);
  buffer->lenght++;
  return 0;
}

int linked_after_add(linked_buffer *buffer, void *data) {
  if (linked_check(buffer))
    return ERR_LINKED_IS_NULL;

  linked_list *p = buffer->head;
  linked_list *node = new_node();
  memcpy(node->data, data, buffer->type_size);

  if (p == NULL) {
    buffer->head = node;
    buffer->tail = node;
  } else {
    buffer->tail->next = node;
    node->prev = buffer->tail;
    buffer->tail = node;
  }

  buffer->lenght++;
  return 0;
}

int linked_get(linked_buffer *buffer, void *data, int index) {
  int err = check_buffer(buffer);
  if (err != 0)
    return err;

  memcpy(data, get_node(buffer, index)->data, buffer->type_size);
  return 0;
}

int linked_modify(linked_buffer *buffer, void *data, int index) {
  int err = check_buffer(buffer);
  if (err != 0)
    return err;

  memcpy(get_node(buffer, index)->data, data, buffer->type_size);
  return 0;
}

int linked_insert(linked_buffer *buffer, void *data, int index) {
  int err = check_buffer(buffer);
  if (err != 0)
    return err;

  if (index == 0)
    linked_first_add(buffer, data);
  else if (index == buffer->lenght - 1)
    linked_after_add(buffer, data);
  else {
    linked_list *curr_node = get_node(buffer, index);
    linked_list *prev_node = curr_node->prev;
    linked_list *node = new_node();

    prev_node->next = node;
    node->prev = prev_node;
    node->next = curr_node;
    curr_node->prev = node;
    memcpy(node->data, data, buffer->type_size);
    buffer->lenght++;
  }

  return 0;
}

int linked_delete(linked_buffer *buffer, int index) {
  int err = check_buffer(buffer);
  if (err != 0)
    return err;

  if (index == 0) {
    linked_list *p = buffer->head;
    p->prev = NULL;
    buffer->head = p->next;
    node_free(p);
  } else {
    linked_list *curr_node = get_node(buffer, index - 1);
    linked_list *prev_node = curr_node->prev;
    prev_node->next = curr_node->next;
    if (prev_node->next == NULL)
      buffer->tail = prev_node;
    else
      prev_node->next->prev = prev_node;

    node_free(curr_node);
  }

  buffer->lenght--;
  return 0;
}

int linked_append(linked_buffer *buffer, linked_buffer *add, int index) {
  if (linked_check(buffer) || linked_check(add))
    return ERR_LINKED_IS_NULL;

  if (buffer->type_size != add->type_size)
    return ERR_LINKED_TYPE_SIZE;

  linked_list *curr_node = get_node(buffer, index);
  linked_list *next_node = curr_node->next;
  curr_node->next = add->head;
  add->head->prev = curr_node;
  add->tail->next = next_node;
  next_node->prev = add->tail;

  buffer->lenght += add->lenght;
  return 0;
}

int linked_check(linked_buffer *head) { return head == NULL; }

void linked_release(int n, ...) {
  va_list list;
  linked_buffer *f;
  linked_list *node, *p;
  va_start(list, n);
  for (int i = 0; i < n; i++) {
    f = va_arg(list, linked_buffer *);
    node = f->head;
    while (node != NULL) {
      p = node;
      node = p->next;
      node_free(p);
    }

    f->head = NULL;
    f->tail = NULL;
    f->lenght = 0;
    f->type_size = 0;
  }

  va_end(list);
}
