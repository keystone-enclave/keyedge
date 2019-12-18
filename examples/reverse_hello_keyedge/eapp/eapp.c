#include "eapp_utils.h"
#include "string.h"
#include "h2ecall.h"
#include "edge_call.h"
#include <syscall.h>
#include "ocalls.h"

void register_inverse_functions();

char* str = "Hello Keyedge!";

msg get_msg() {
	msg ret;
	ret.content = str;
	ret.size = strlen(str) + 1;
	return ret;
}

void EAPP_ENTRY eapp_entry() {
	register_inverse_functions();
	receive_calls(h2ecall_dispatch);
}

