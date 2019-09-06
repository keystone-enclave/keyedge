#include "common_header.h"
#include "hello_keyedge.keg.eapp.h"

void _start(){
	int ret = print_string((signed char *) "Hello Keyedge!\n");
	print_int(ret);
	EAPP_RETURN(0);
}

