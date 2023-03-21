#include "buffer.h"
#include <memory.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#define __new_a_bufferer(cap) malloc(cap)
int expansion(Buffer *src);

// Let's new a bufferer
// When creating a bufferer, you need to be clear about the byte size of the
// type you want to store.
Buffer new_buffer(long cap, long type_size) {
  Buffer s;
  s.buffer = NULL;

  if (cap < 0) {
    cap = 0;
  } else {
    s.buffer = __new_a_bufferer(cap * type_size);
  }

  s.cap = cap;
  s.lenght = 0;
  s.type_size = type_size;
  return s;
}

// Append a bufferer to the end of the bufferer.
int buffer_append(Buffer *src, Buffer *data) {
  if (src->type_size != data->type_size) {
    return ERR_TYPE_SIZE_NOT_EQUAL;
  }

  char elem[src->type_size];
  int err;
  for (int i = 0; i < data->lenght; i++) {
    if ((err = buffer_get(data, elem, i)) != 0)
      return err;
    if ((err = buffer_add(src, elem)) != 0)
      return err;
  }
  return 0;
};

// Add an element to the end of the bufferer.
int buffer_add(Buffer *s, void *const data) {
  int err;
  if ((s->lenght >= s->cap || buffer_null(s)) && (err = expansion(s) != 0)) {
    return err;
  }

  memcpy(s->buffer + s->lenght++ * s->type_size, data, s->type_size);
  return 0;
}

// Retrieve an element based on its index
// you can use negative numbers to retrieve elements from the end of the
// bufferer, for example: -1 retrieves the last element
int buffer_get(Buffer *s, void *const data, long index) {
  if (buffer_null(s)) {
    return ERR_NULL_POINTER;
  }
  if (s->lenght == 0) {
    return ERR_ZERO_LENGHT;
  }
  if (index > s->lenght) {
    return ERR_INVALID_INDEX;
  }
  if (index < 0) {
    index += s->lenght;
  }

  memcpy(data, s->buffer + index * s->type_size, s->type_size);
  return 0;
}

// Slice bufferer
Buffer buffer_slice(Buffer *s, long start, long end) {
  Buffer ns = new_buffer(end - start + 1, s->type_size);
  if (s->lenght == 0 || start < 0 || end < 0 || start > end ||
      end > s->lenght || start > s->lenght) {
    buffer_release(1, &ns);
    return ns;
  }

  char elem[s->type_size];
  int err;
  while (start < end) {
    buffer_get(s, elem, start++);
    if ((err = buffer_add(&ns, elem)) != 0) {
      buffer_release(1, &ns);
      break;
    }
  }
  return ns;
}

// Delete the values between start and end
int buffer_delete(Buffer *s, Buffer *news, long start, long end) {
  int err;
  Buffer st = buffer_slice(s, 0, start);
  Buffer en = buffer_slice(s, --end, s->lenght);
  Buffer nb = new_buffer(st.lenght + en.lenght, s->type_size);
  if ((err = buffer_append(&nb, &st)) != 0) {
    buffer_release(3, &nb, &st, en);
    return err;
  }

  if ((err = buffer_append(&nb, &en)) != 0) {
    buffer_release(3, &nb, &st, en);
    return err;
  }

  *news = nb;
  return 0;
}

// Insert an element between the indices
// if the start and end numbers are not adjacent and represent a range, the
// elements within this range will be deleted before inserting the new element.
int buffer_insert(Buffer *s, void *const data, Buffer *news, long start,
                  long end) {
  Buffer st = buffer_slice(s, 0, start);
  Buffer en = buffer_slice(s, --end, s->lenght);
  Buffer nb = new_buffer(st.lenght + en.lenght, s->type_size);
  int err;
  if ((err = buffer_append(&nb, &st)) != 0) {
    buffer_release(3, &nb, &st, en);
    return err;
  }

  if ((err = buffer_add(&nb, data)) != 0) {
    buffer_release(3, &nb, &st, en);
    return err;
  }

  if ((err = buffer_append(&nb, &en)) != 0) {
    buffer_release(3, &nb, &st, en);
    return err;
  }

  *news = nb;
  return 0;
}

// Insert a bufferer
// if the start and end numbers are not adjacent and represent a range, the
// elements within this range will be deleted before inserting the new element.
int buffer_imore(Buffer *s, Buffer *data, Buffer *news, long start, long end) {
  int err;
  Buffer st = buffer_slice(s, 0, start);
  Buffer en = buffer_slice(s, --end, s->lenght);
  Buffer nb = new_buffer(st.lenght + en.lenght, s->type_size);
  if ((err = buffer_append(&nb, &st)) != 0) {
    buffer_release(3, &nb, &st, en);
    return err;
  }

  if ((err = buffer_append(&nb, data)) != 0) {
    buffer_release(3, &nb, &st, en);
    return err;
  }

  if ((err = buffer_append(&nb, &en)) != 0) {
    buffer_release(3, &nb, &st, en);
    return err;
  }

  *news = nb;
  return 0;
}

// Clone the buffer
int buffer_clone(Buffer *src, Buffer *news) {
  int err;
  Buffer nb = new_buffer(src->lenght, src->type_size);

  if ((err = buffer_append(&nb, src)) != 0) {
    buffer_release(1, &nb);
    return err;
  }

  *news = nb;
  return 0;
}

// Check if the buffer is equal to null
int buffer_null(Buffer *src) {
  if (src == NULL)
    return 1;
  return src->buffer == NULL;
}

// Return buffer pointer
void *buffer_ptr(Buffer *src) {
  if (buffer_null(src))
    return NULL;
  return src->buffer;
}

// Release some bufferer.
void buffer_release(int num, ...) {
  va_list list;
  Buffer *f;
  va_start(list, num);
  for (size_t i = 0; i < num; i++) {
    f = va_arg(list, Buffer *);
    free(f->buffer);
    f->buffer = NULL;
    f->lenght = 0;
    f->cap = 0;
    f->type_size = 0;
  }
  va_end(list);
}

// dynamic expansion
int expansion(Buffer *src) {
  long oldcap = src->cap;
  long newcap = oldcap;

  if (newcap < 1)
    newcap = 1;
  else if (newcap < 1024)
    newcap *= 2;
  else
    newcap = newcap * 1.25;

  void *newp = __new_a_bufferer(newcap * src->type_size);
  if (newp == NULL)
    return ERR_ALLOCATION_FAILED;

  memcpy(newp, src->buffer, oldcap * src->type_size);
  if (oldcap)
    free(src->buffer);

  src->cap = newcap;
  src->buffer = newp;
  return 0;
}
