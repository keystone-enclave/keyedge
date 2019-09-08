#include "common_header.h"
#include "hello_keyedge.keg.eapp.h"
#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>

signed char str[50] = "Hello Keyedge!";

void EAPP_ENTRY eapp_entry() {
	int ret = print_string(str);
	print_int(ret);
	EAPP_RETURN(0);
}

