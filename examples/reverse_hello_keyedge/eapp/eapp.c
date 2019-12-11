#include "common_header.h"
#include "ocalls_eapp.h"

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

