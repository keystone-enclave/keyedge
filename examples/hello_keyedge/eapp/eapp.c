#include "eapp_utils.h"
#include "string.h"
#include "h2ecall.h"
#include "edge_call.h"
#include <syscall.h>
#include "ocalls.h"

void register_inverse_functions();

char* str = "Hello Keyedge!";

void EAPP_ENTRY eapp_entry() {
	int ret = print_string(str);
	print_return_value(ret);
	EAPP_RETURN(0);
}

