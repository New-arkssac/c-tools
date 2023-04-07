// buffer string processing

#ifndef __BUFF_EXP
#include "array.h"

int array_perfix(array_buffer s, array_buffer perfix);
int array_suffix(array_buffer s, array_buffer suffix);
int array_index(array_buffer s, array_buffer sep);
int array_contains(array_buffer s, array_buffer substr);
int array_count(array_buffer s, array_buffer substr);
int array_cut(array_buffer s, array_buffer sep, array_buffer *result);
int array_join(array_buffer s, array_buffer sep, array_buffer *result);
#endif /* ifndef __BUFF_EXP */
