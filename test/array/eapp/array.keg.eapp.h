#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>
const unsigned long __function_print_messages = 1;
int print_messages(int count, char msg[5][50]) {
	char __argument_space[0 + (sizeof(int)) + (((sizeof(char)) * (50)) * (5))];
	char __return_space[sizeof(int)];
	*((int *) (__argument_space + (0))) = count;
	for (size_t i1 = 0; i1 < (5); ++i1)
		for (size_t i2 = 0; i2 < (50); ++i2)
			*((char *) (__argument_space + (0 + (sizeof(int)))) + (i1) * ((sizeof(char)) * (50)) + (i2) * (sizeof(char))) = msg[i1][i2];
	ocall (__function_print_messages, __argument_space, 0 + (sizeof(int)) + (((sizeof(char)) * (50)) * (5)), __return_space, sizeof(int));
	return *((int *) __return_space);
}
