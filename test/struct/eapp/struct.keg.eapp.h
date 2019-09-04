#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>
typedef struct {
	int h;
	int m;
	int s;
} tim;
typedef struct {
	int h;
	int m;
	int s;
} __serialized_tim;
typedef struct {
	tim t;
	char sender[10];
	char content[50];
} message;
typedef struct {
	__serialized_tim t;
	char sender[10];
	char content[50];
} __serialized_message;
const unsigned long __function_print_messages = 1;
int print_messages(int count, message m[5]) {
	char __argument_space[0 + (sizeof(int)) + ((sizeof(__serialized_message)) * (5))];
	char __return_space[sizeof(int)];
	*((int *) (__argument_space + (0))) = count;
	for (size_t i1 = 0; i1 < (5); ++i1) {
		// serializing message
			// serializing tim
				*((int *) &((__serialized_tim*) (&((__serialized_message*) ((__argument_space + (0 + (sizeof(int)))) + (i1) * (sizeof(__serialized_message)))) -> t)) -> h) = m[i1].t.h;
				*((int *) &((__serialized_tim*) (&((__serialized_message*) ((__argument_space + (0 + (sizeof(int)))) + (i1) * (sizeof(__serialized_message)))) -> t)) -> m) = m[i1].t.m;
				*((int *) &((__serialized_tim*) (&((__serialized_message*) ((__argument_space + (0 + (sizeof(int)))) + (i1) * (sizeof(__serialized_message)))) -> t)) -> s) = m[i1].t.s;
			for (size_t i3 = 0; i3 < (10); ++i3) {
				*((char *) &((__serialized_message*) ((__argument_space + (0 + (sizeof(int)))) + (i1) * (sizeof(__serialized_message)))) -> sender + (i3) * (sizeof(char))) = m[i1].sender[i3];
			}
			for (size_t i3 = 0; i3 < (50); ++i3) {
				*((char *) &((__serialized_message*) ((__argument_space + (0 + (sizeof(int)))) + (i1) * (sizeof(__serialized_message)))) -> content + (i3) * (sizeof(char))) = m[i1].content[i3];
			}
	}
	ocall (__function_print_messages, __argument_space, 0 + (sizeof(int)) + ((sizeof(__serialized_message)) * (5)), __return_space, sizeof(int));
	return *((int *) __return_space);
}
