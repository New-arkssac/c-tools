#ifndef __LINK_BUFFER

enum {
  ERR_NULL_POINTER = 1,
  ERR_LINK_IS_ZERO,
  ERR_INVALID_INDEX,
  ERR_LINK_IS_NULL,
  ERR_LINK_TYPE_SIZE,
};

typedef struct link_buffer link_buffer;
typedef struct link_head link_head;

struct link_head {
  link_buffer *next;
  long lenght;
  long type_size;
};

struct link_buffer {
  void *data;
  link_buffer *next;
};

link_head new_head(long type_size);
int link_append(link_head *head, link_head *data, int index);
int link_add(link_head *head, void *data);
int link_replace(link_head *head, void *data, int index);
int link_get(link_head *head, void *data, int index);
int link_insert(link_head *head, void *data, int index);
int link_delete(link_head *head, int index);
int link_check(link_head *head);
void link_release(int n, ...);
#endif /* ifndef __LINK_BUFFER */
