#include "common_header.h"
#include "ocalls_eapp.h"

char* str = "Hello Keyedge!";

void EAPP_ENTRY eapp_entry() {
	int ret = print_string(str);
	print_return_value(ret);
	EAPP_RETURN(0);
}

