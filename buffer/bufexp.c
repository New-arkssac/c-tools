#include "bufexp.h"
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int hash_buffer(Buffer *string);
int utf8_buffer(const char byte);

int buffer_perfix(Buffer s, Buffer perfix) {
  int s_len = s.lenght;
  int perfix_len = perfix.lenght;

  if (s_len == 0 || perfix_len == 0 || s_len < perfix_len || buffer_null(&s)) {
    return 0;
  }

  Buffer sep = s;

  char n, m;
  for (int i = 0; i < perfix_len; i++) {
    buffer_get(&sep, &n, i);
    buffer_get(&perfix, &m, i);

    if (n != m) {
      return 0;
    }
  }

  return 1;
}

int buffer_suffix(Buffer s, Buffer suffix) {
  int s_len = s.lenght;
  int suffix_len = suffix.lenght;

  if (s_len == 0 || suffix_len == 0 || s_len < suffix_len || buffer_null(&s)) {
    return 0;
  }

  Buffer sep = s;

  sep.buffer = s.buffer + (s_len - suffix_len) * s.type_size;

  char n, m;
  for (int i = 0; i < suffix_len; i++) {
    buffer_get(&sep, &n, i);
    buffer_get(&suffix, &m, i);

    if (n != m) {
      return 0;
    }
  }

  return 1;
}

int buffer_index(Buffer s, Buffer sep) {
  int s_len = s.lenght;
  int sep_len = sep.lenght;
  if (s_len <= 0 || sep_len <= 0) {
    return -1;
  }

  int sep_hash = hash_buffer(&sep), s_hash = 0, k, j;
  void *ma = buffer_ptr(&s);
  Buffer sh = sep;

  char c1, c2;
  for (int i = 0, sp; i <= s_len - sep_len; i += sp) {
    sp = utf8_buffer(*(char *)(ma + i));
    if (sp == -1) {
      break;
    }

    sh.buffer = ma + i;
    s_hash = hash_buffer(&sh);

    if (s_hash != sep_hash)
      continue;

    for (j = 0; j < sep_len; j++) {
      buffer_get(&sh, &c1, j);
      buffer_get(&sep, &c2, j);

      if (c1 != c2)
        break;
    }

    if (j == sep_len) {
      return i;
    }
  }

  return -1;
}

int buffer_contains(Buffer s, Buffer substr) {
  return buffer_index(s, substr) >= 0;
}

int buffer_count(Buffer s, Buffer sep) {
  int sep_len = sep.lenght;
  if (sep_len <= 0) {
    return -1;
  }

  int num = 0;
  Buffer sub;
  buffer_clone(&s, &sub);

  for (int n = 0, i = 0;;) {
    n = buffer_index(sub, sep);
    buffer_release(1, &sub);
    if (n == -1) {
      return num;
    }

    num++;
    i += n + sep_len;
    sub = buffer_slice(&s, i, s.lenght);
  }
}

int buffer_cut(Buffer s, Buffer sep, Buffer *result) {
  int i = buffer_index(s, sep);
  if (i >= 0) {
    int err;
    Buffer bk = new_buffer(2, sizeof(Buffer));
    Buffer before = buffer_slice(&s, 0, i);
    Buffer after = buffer_slice(&s, i + sep.lenght, s.lenght);
    if ((err = buffer_add(&bk, &before)) != 0) {
      buffer_release(3, &bk, &before, &after);
      return err;
    }

    if ((err = buffer_add(&bk, &after)) != 0) {
      buffer_release(3, &bk, &before, &after);
      return err;
    }

    *result = bk;
    return 0;
  }

  return 1;
}

int buffer_join(Buffer s, Buffer sep, Buffer *result) {
  if (s.type_size != sizeof(Buffer)) {
    return ERR_TYPE_SIZE_NOT_EQUAL;
  }

  switch (s.lenght) {
  case 0:
    *result = new_buffer(0, sep.type_size);
    return 1;
  case 1:
    buffer_get(&s, &result, 0);
    break;
  }

  Buffer tp;
  int err, n = s.lenght * (sep.lenght - 1);
  for (int i = 0; i < s.lenght; i++) {
    if ((err = buffer_get(&s, &tp, i)) != 0)
      return 1;
    n += tp.lenght;
  }

  Buffer string = new_buffer(n, sizeof(char));
  buffer_get(&s, &tp, 0);
  if ((err = buffer_append(&string, &tp)) != 0) {
    buffer_release(1, &string);
    return 1;
  }

  for (int i = 1; i < s.lenght; i++) {
    if ((err = buffer_append(&string, &sep)) != 0) {
      buffer_release(1, &string);
      return 1;
    }

    buffer_get(&s, &tp, i);
    if ((err = buffer_append(&string, &tp)) != 0) {
      buffer_release(1, &string);
      return 1;
    }
  }

  *result = string;
  return 0;
}

unsigned int hash_buffer(Buffer *s) {
  const unsigned int prime_RK = 16777619;
  unsigned int hash = 0;
  char elem[s->type_size];
  for (int i = 0; i < s->lenght; i++) {
    if (buffer_get(s, elem, i) != 0) {
      return -1;
    };
    hash = (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
    hash += *elem;
  }

  unsigned int pow = 1, sp = prime_RK;
  for (int i = s->lenght; i > 0; i >>= 1) {
    if ((i & 1) != 0) {
      pow *= sp;
    }
    sp *= sp;
  }

  return hash ^ pow;
}

// int utf8_rune(const char byte) {
int utf8_buffer(const char byte) {
  unsigned char bt = byte;
  int len = 0;
  if ((bt & 0x80) == 0x00) {
    len = 1;
  } else if ((bt & 0xE0) == 0xC0) {
    len = 2;
  } else if ((bt & 0xF0) == 0xE0) {
    len = 3;
  } else if ((bt & 0xF8) == 0xF0) {
    len = 4;
  } else {
    return -1;
  }

  return len;
}
