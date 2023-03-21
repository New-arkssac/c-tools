#ifndef __BUFF_EXP
#include "buffer.h"

typedef int rune;
int buffer_perfix(Buffer s, Buffer perfix);
int buffer_suffix(Buffer s, Buffer suffix);
int buffer_index(Buffer s, Buffer sep);
int buffer_contains(Buffer s, Buffer substr);
int buffer_count(Buffer s, Buffer substr);
int buffer_cut(Buffer s, Buffer sep, Buffer *result);
int buffer_join(Buffer s, Buffer sep, Buffer *result);

#endif /* ifndef __BUFF_EXP */
