#include "array.h"
#include <memory.h>
#include <stdarg.h>

#define __new_a_bufferer(cap) malloc(cap)
#define __array_check(buffer)                                                  \
  ({                                                                           \
    array_null(s)       ? ERR_NULL_POINTER                                     \
    : s->lenght == 0    ? ERR_ZERO_LENGHT                                      \
    : index > s->lenght ? ERR_INVALID_INDEX                                    \
                        : 0;                                                   \
  })
int expansion(array_buffer *src);

// Let's new a bufferer
// When creating a bufferer, you need to be clear about the byte size of the
// type you want to store.
array_buffer new_array(long cap, long type_size) {
  if (cap < 0)
    cap = 0;

  array_buffer s = {
      .buffer = __new_a_bufferer(cap * type_size),
      .lenght = 0,
      .cap = cap,
      .type_size = type_size,
  };

  return s;
}

// Append a bufferer to the end of the bufferer.
int array_append(array_buffer *src, array_buffer *data) {
  if (src->type_size != data->type_size)
    return ERR_TYPE_SIZE_NOT_EQUAL;

  char elem[src->type_size];
  int err = 0;
  for (int i = 0; i < data->lenght; i++) {
    if ((err = array_get(data, elem, i)) != 0)
      break;
    if ((err = array_add(src, elem)) != 0)
      break;
  }

  return err;
};

// Add an element to the end of the bufferer.
int array_add(array_buffer *s, void *const data) {
  int err;
  if ((s->lenght >= s->cap || array_null(s)) && (err = expansion(s) != 0))
    return err;

  memcpy(s->buffer + s->lenght++ * s->type_size, data, s->type_size);
  return 0;
}

// Retrieve an element based on its index
// you can use negative numbers to retrieve elements from the end of the
// bufferer, for example: -1 retrieves the last element
int array_get(array_buffer *s, void *const data, long index) {
  int err = __array_check(s);
  if (err != 0)
    return err;

  if (index < 0)
    index += s->lenght;

  memcpy(data, s->buffer + index * s->type_size, s->type_size);
  return 0;
}

int array_modify(array_buffer *s, void *const data, long index) {
  int err = __array_check(s);
  if (err != 0)
    return err;

  if (index < 0)
    index += s->lenght;

  memcpy(s->buffer + index * s->type_size, data, s->type_size);
  return 0;
}

// Slice bufferer
array_buffer array_slice(array_buffer *s, long start, long end) {
  array_buffer ns = new_array(end - start + 1, s->type_size);
  if (s->lenght == 0 || start < 0 || end < 0 || start > end ||
      end > s->lenght || start > s->lenght) {
    array_release(&ns);
    return ns;
  }

  char elem[s->type_size];
  int err;
  while (start < end) {
    array_get(s, elem, start++);
    if ((err = array_add(&ns, elem)) != 0) {
      array_release(&ns);
      break;
    }
  }
  return ns;
}

// Delete the values between start and end
int array_delete(array_buffer *s, array_buffer *news, long start, long end) {
  int err;
  array_buffer st = array_slice(s, 0, start);
  array_buffer en = array_slice(s, --end, s->lenght);
  array_buffer nb = new_array(st.lenght + en.lenght, s->type_size);
  if ((err = array_append(&nb, &st)) != 0) {
    array_release(&nb, &st, &en);
    return err;
  }

  if ((err = array_append(&nb, &en)) != 0) {
    array_release(&nb, &st, &en);
    return err;
  }

  *news = nb;
  return 0;
}

// Insert an element between the indices
// if the start and end numbers are not adjacent and represent a range, the
// elements within this range will be deleted before inserting the new element.
int array_insert(array_buffer *s, void *const data, array_buffer *news,
                 long start, long end) {
  array_buffer st = array_slice(s, 0, start);
  array_buffer en = array_slice(s, --end, s->lenght);
  array_buffer nb = new_array(st.lenght + en.lenght, s->type_size);
  int err;
  if ((err = array_append(&nb, &st)) != 0) {
    array_release(&nb, &st, &en);
    return err;
  }

  if ((err = array_add(&nb, data)) != 0) {
    array_release(&nb, &st, &en);
    return err;
  }

  if ((err = array_append(&nb, &en)) != 0) {
    array_release(&nb, &st, &en);
    return err;
  }

  *news = nb;
  return 0;
}

// Insert a bufferer
// if the start and end numbers are not adjacent and represent a range, the
// elements within this range will be deleted before inserting the new element.
int array_imore(array_buffer *s, array_buffer *data, array_buffer *news,
                long start, long end) {
  int err;
  array_buffer st = array_slice(s, 0, start);
  array_buffer en = array_slice(s, --end, s->lenght);
  array_buffer nb = new_array(st.lenght + en.lenght, s->type_size);
  if ((err = array_append(&nb, &st)) != 0) {
    array_release(&nb, &st, &en);
    return err;
  }

  if ((err = array_append(&nb, data)) != 0) {
    array_release(&nb, &st, &en);
    return err;
  }

  if ((err = array_append(&nb, &en)) != 0) {
    array_release(&nb, &st, &en);
    return err;
  }

  *news = nb;
  return 0;
}

// Clone the buffer
int array_clone(array_buffer *src, array_buffer *news) {
  int err;
  array_buffer nb = new_array(src->lenght, src->type_size);

  if ((err = array_append(&nb, src)) != 0) {
    array_release(&nb);
    return err;
  }

  *news = nb;
  return 0;
}

// Check if the buffer is equal to null
int array_null(array_buffer *src) { return src == NULL || src->buffer == NULL; }

// Return buffer pointer
void *array_ptr(array_buffer *src) {
  return (array_null(src) ? NULL : src->buffer);
}

// dynamic expansion
int expansion(array_buffer *src) {
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
