#include "link.h"
#include <memory.h>
#include <stdarg.h>
#include <stdlib.h>

#define new_data(cap) malloc(cap);
#define new_link(cap) malloc(cap);

link_head new_head(long type_size) {
  link_head head = {
      .next = NULL,
      .lenght = 0,
      .type_size = type_size,
  };

  return head;
}

int link_add(link_head *head, void *data) {
  if (link_check(head))
    return ERR_LINK_IS_NULL;

  link_buffer *p = head->next;
  link_buffer *nlink = new_link(sizeof(link_buffer));
  if (nlink == NULL)
    return ERR_NULL_POINTER;

  nlink->data = new_data(head->type_size);
  if (nlink->data == NULL) {
    free(nlink);
    return ERR_NULL_POINTER;
  }

  memcpy(nlink->data, data, head->type_size);
  nlink->next = NULL;

  if (p == NULL) {
    head->next = nlink;
  } else {
    while (p->next != NULL)
      p = p->next;
    p->next = nlink;
  }

  head->lenght++;
  return 0;
}

int link_get(link_head *head, void *data, int index) {
  if (link_check(head))
    return ERR_LINK_IS_NULL;

  if (head->lenght == 0)
    return ERR_LINK_IS_ZERO;

  if (index >= head->lenght)
    return ERR_INVALID_INDEX;

  link_buffer *p = head->next;
  while (index--)
    p = p->next;

  memcpy(data, p->data, head->type_size);
  return 0;
}

int link_replace(link_head *head, void *data, int index) {
  if (link_check(head))
    return ERR_LINK_IS_NULL;

  if (head->lenght == 0)
    return ERR_LINK_IS_ZERO;

  if (index >= head->lenght)
    return ERR_INVALID_INDEX;

  link_buffer *p = head->next;
  while (index--)
    p = p->next;

  memcpy(p->data, data, head->type_size);
  return 0;
}

int link_insert(link_head *head, void *data, int index) {
  if (link_check(head))
    return ERR_LINK_IS_NULL;

  if (head->lenght == 0)
    return ERR_LINK_IS_ZERO;

  if (index >= head->lenght)
    return ERR_INVALID_INDEX;

  link_buffer *p = head->next;
  link_buffer *prev = NULL;
  while (index--) {
    prev = p;
    p = p->next;
  }

  link_buffer *nlink = new_link(sizeof(link_buffer));
  if (nlink == NULL)
    return ERR_NULL_POINTER;

  nlink->data = new_data(head->type_size);
  if (nlink->data == NULL) {
    free(nlink);
    return ERR_NULL_POINTER;
  }

  nlink->next = prev->next;
  prev->next = nlink;
  head->lenght++;

  memcpy(nlink->data, data, head->type_size);
  return 0;
}

int link_delete(link_head *head, int index) {
  if (head->lenght == 0)
    return ERR_LINK_IS_ZERO;

  if (index >= head->lenght)
    return ERR_INVALID_INDEX;

  link_buffer *p = head->next;
  link_buffer *prev = NULL;
  while (index--) {
    prev = p;
    p = p->next;
  }

  prev->next = p->next;
  free(p->data);
  free(p);

  head->lenght--;
  return 0;
}

int link_append(link_head *head, link_head *data, int index) {
  if (link_check(head) || link_check(data))
    return ERR_LINK_IS_NULL;

  if (head->type_size != data->type_size)
    return ERR_LINK_TYPE_SIZE;

  link_buffer *p = head->next;
  link_buffer *prev = NULL;
  while (p != NULL && index--) {
    prev = p;
    p = p->next;
  }

  if (prev == NULL) {
    head->next = data->next;
  } else {
    prev->next = data->next;
  }

  head->lenght += data->lenght;
  return 0;
}

int link_check(link_head *head) { return head == NULL; }

void link_release(int n, ...) {
  va_list list;
  link_head *f;
  link_buffer *next, *curr;
  va_start(list, n);
  for (int i = 0; i < n; i++) {
    f = va_arg(list, link_head *);
    next = f->next;
    if (next == NULL)
      continue;

    while (next != NULL) {
      curr = next->next;
      free(next->data);
      free(next);
      next = curr;
    }
  }
  va_end(list);
}
