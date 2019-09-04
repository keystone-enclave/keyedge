#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>
const unsigned long __function_print_message = 1;
int print_message(char msg[50]) {
	char __argument_space[0 + ((sizeof(char)) * (50))];
	char __return_space[sizeof(int)];
	for (size_t i1 = 0; i1 < (50); ++i1)
		*((char *) (__argument_space + (0)) + (i1) * (sizeof(char))) = msg[i1];
	ocall (__function_print_message, __argument_space, 0 + ((sizeof(char)) * (50)), __return_space, sizeof(int));
	return *((int *) __return_space);
}
