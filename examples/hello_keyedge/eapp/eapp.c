#include "common_header.h"
#include "ocalls_eapp.h"
#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>

char* str = "Hello Keyedge!";

void EAPP_ENTRY eapp_entry() {
	int ret = print_string(str, strlen(str) + 1);
	print_return_value(ret);
	EAPP_RETURN(0);
}

