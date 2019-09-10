#include "common_header.h"
#include "pointer.keg.eapp.h"
#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>

void EAPP_ENTRY eapp_entry() {
	int p = 42;
	int *a = &p;
	int *b = NULL;
	print_pointer('a', a);
	print_pointer('b', b);
	EAPP_RETURN(0);
}

