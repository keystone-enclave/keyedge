#include <keystone.h>
#include <edge_call.h>
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
int print_messages(int count, message m[5]);
void __wrapper_print_messages(void* buffer) {
	struct edge_call* edge_call = (struct edge_call*) buffer;
	uintptr_t call_args;
	size_t arg_len;
	if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;
		return;
	}
	int count;
	message m[5];
	count = *((int *) ((char *) call_args + 0));
	for (size_t i1 = 0; i1 < (5); ++i1) {
		// de-serializing message
			// de-serializing tim
				m[i1].t.h = *((int *) &((__serialized_tim*) (&((__serialized_message*) (((char *) call_args + 0 + (sizeof(int))) + (i1) * (sizeof(__serialized_message)))) -> t)) -> h);
				m[i1].t.m = *((int *) &((__serialized_tim*) (&((__serialized_message*) (((char *) call_args + 0 + (sizeof(int))) + (i1) * (sizeof(__serialized_message)))) -> t)) -> m);
				m[i1].t.s = *((int *) &((__serialized_tim*) (&((__serialized_message*) (((char *) call_args + 0 + (sizeof(int))) + (i1) * (sizeof(__serialized_message)))) -> t)) -> s);
			for (size_t i3 = 0; i3 < (10); ++i3) {
				m[i1].sender[i3] = *((char *) &((__serialized_message*) (((char *) call_args + 0 + (sizeof(int))) + (i1) * (sizeof(__serialized_message)))) -> sender + (i3) * (sizeof(char)));
			}
			for (size_t i3 = 0; i3 < (50); ++i3) {
				m[i1].content[i3] = *((char *) &((__serialized_message*) (((char *) call_args + 0 + (sizeof(int))) + (i1) * (sizeof(__serialized_message)))) -> content + (i3) * (sizeof(char)));
			}
	}
	int ret_val;
	ret_val = print_messages(count, m);
	uintptr_t data_section = edge_call_data_ptr();
	*((int *) ((char *) data_section)) = ret_val;
	if (edge_call_setup_ret(edge_call, (void*) data_section, sizeof(int))) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_PTR;
	} else {
		edge_call -> return_data.call_status = CALL_STATUS_OK;
	}
	return;
}
void register_functions() {
	const unsigned long __function_print_messages = 1;
	register_call(__function_print_messages, __wrapper_print_messages);
	return;
}
