#include "common_header.h"
#include "hello_keyedge.keg.eapp.h"

char memory[2048];
void *__malloc_start = memory;

void _start(){
	int ret = print_string((signed char *) "Hello Keyedge!\n");
	print_int(ret);
	EAPP_RETURN(0);
}

