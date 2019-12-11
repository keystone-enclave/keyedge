#include <ocalls_header.h>

typedef struct msg {
	keyedge_vla char* content;
	keyedge_size int size;
} msg;

msg get_msg() keyedge_inverse;

