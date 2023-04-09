#include "strexp.h"
#include "array.h"

unsigned int hash_buffer(array_buffer *string);
int utf8_buffer(const char byte);

// Check if a substring is a prefix of the main string.
// Return 1 if it exists, 0 if it does not.
int array_perfix(array_buffer s, array_buffer perfix) {
  int s_len = s.lenght;
  int perfix_len = perfix.lenght;

  if (s_len == 0 || perfix_len == 0 || s_len < perfix_len || !array_valid(&s))
    return 0;

  array_buffer sep = s;

  char n, m;
  for (int i = 0; i < perfix_len; i++) {
    array_get(&sep, &n, i);
    array_get(&perfix, &m, i);

    if (n != m)
      return 0;
  }

  return 1;
}

// Check if a substring is a suffix of the main string.
// Return 1 if it exists, 0 if it does not.
int array_suffix(array_buffer s, array_buffer suffix) {
  int s_len = s.lenght;
  int suffix_len = suffix.lenght;

  if (s_len == 0 || suffix_len == 0 || s_len < suffix_len || !array_valid(&s))
    return 0;

  array_buffer sep = s;

  sep.buffer = s.buffer + (s_len - suffix_len) * s.type_size;

  char n, m;
  for (int i = 0; i < suffix_len; i++) {
    array_get(&sep, &n, i);
    array_get(&suffix, &m, i);

    if (n != m)
      return 0;
  }

  return 1;
}

// Return the index where the substring appears in the main string, return -1 if
// not found. Use the RK algorithm.
int array_index(array_buffer s, array_buffer sep) {
  int s_len = s.lenght;
  int sep_len = sep.lenght;
  if (s_len <= 0 || sep_len <= 0)
    return -1;

  int sep_hash = hash_buffer(&sep), s_hash = 0, k, j;
  void *ma = array_ptr(&s);
  array_buffer sh = sep;

  char c1, c2;
  for (int i = 0, sp; i <= s_len - sep_len; i += sp) {
    sp = utf8_buffer(*(char *)(ma + i));
    if (sp == -1)
      break;

    sh.buffer = ma + i;
    s_hash = hash_buffer(&sh);

    if (s_hash != sep_hash)
      continue;

    for (j = 0; j < sep_len; j++) {
      array_get(&sh, &c1, j);
      array_get(&sep, &c2, j);

      if (c1 != c2)
        break;
    }

    if (j == sep_len)
      return i;
  }

  return -1;
}

// Return 1 if the substring appears in the main string, 0 if not.
int array_contains(array_buffer s, array_buffer substr) {
  return array_index(s, substr) >= 0;
}

// Count the number of times the substring appears in the main string.
int array_count(array_buffer s, array_buffer sep) {
  int sep_len = sep.lenght;
  if (sep_len <= 0)
    return -1;

  int num = 0;
  array_buffer sub;
  array_clone(&s, &sub);

  for (int n = 0, i = 0;;) {
    n = array_index(sub, sep);
    array_release(&sub);
    if (n == -1)
      return num;

    num++;
    i += n + sep_len;
    sub = array_slice(&s, i, s.lenght);
  }
}

// Split the position of the substring in the main string. If the substring
// exists, assign the nested before and after strings after cutting to the
// parameter *result.
int array_cut(array_buffer s, array_buffer sep, array_buffer *result) {
  int i = array_index(s, sep);
  if (i >= 0) {
    int err;
    array_buffer bk = new_array(2, sizeof(array_buffer));
    array_buffer before = array_slice(&s, 0, i);
    array_buffer after = array_slice(&s, i + sep.lenght, s.lenght);
    if ((err = array_add(&bk, &before)) != 0) {
      array_release(&bk, &before, &after);
      return err;
    }

    if ((err = array_add(&bk, &after)) != 0) {
      array_release(&bk, &before, &after);
      return err;
    }

    *result = bk;
    return 0;
  }

  return 1;
}

// Concatenate strings in the buffer with sep to form a single buffer.
int array_join(array_buffer s, array_buffer sep, array_buffer *result) {
  if (s.type_size != sizeof(array_buffer))
    return ERR_TYPE_SIZE_NOT_EQUAL;

  switch (s.lenght) {
  case 0:
    *result = new_array(0, sep.type_size);
    return 1;
  case 1:
    array_get(&s, &result, 0);
    break;
  }

  array_buffer tp;
  int err, n = s.lenght * (sep.lenght - 1);
  for (int i = 0; i < s.lenght; i++) {
    if ((err = array_get(&s, &tp, i)) != 0)
      return 1;
    n += tp.lenght;
  }

  array_buffer string = new_array(n, sizeof(char));
  array_get(&s, &tp, 0);
  if ((err = array_append(&string, &tp)) != 0) {
    array_release(&string);
    return 1;
  }

  for (int i = 1; i < s.lenght; i++) {
    if ((err = array_append(&string, &sep)) != 0) {
      array_release(&string);
      return 1;
    }

    array_get(&s, &tp, i);
    if ((err = array_append(&string, &tp)) != 0) {
      array_release(&string);
      return 1;
    }
  }

  *result = string;
  return 0;
}

// The hash function required for the RK algorithm.
unsigned int hash_buffer(array_buffer *s) {
  const unsigned int prime_RK = 16777619;
  unsigned int hash = 0;
  char elem[s->type_size];
  for (int i = 0; i < s->lenght; i++) {
    if (array_get(s, elem, i) != 0)
      return -1;

    hash = (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
    hash += *elem;
  }

  unsigned int pow = 1, sp = prime_RK;
  for (int i = s->lenght; i > 0; i >>= 1) {
    if ((i & 1) != 0)
      pow *= sp;

    sp *= sp;
  }

  return hash ^ pow;
}

// Check if a character is encoded in UTF-8 and return the corresponding
// character sequence.
int utf8_buffer(const char byte) {
  unsigned char bt = byte;
  int len = -1;
  if ((bt & 0x80) == 0x00)
    len = 1;
  else if ((bt & 0xE0) == 0xC0)
    len = 2;
  else if ((bt & 0xF0) == 0xE0)
    len = 3;
  else if ((bt & 0xF8) == 0xF0)
    len = 4;

  return len;
}
