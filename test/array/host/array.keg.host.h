#include <keystone.h>
#include <edge_call.h>
int print_messages(int count, char msg[5][50]);
void __wrapper_print_messages(void* buffer) {
	struct edge_call* edge_call = (struct edge_call*) buffer;
	uintptr_t call_args;
	size_t arg_len;
	if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;
		return;
	}
	int count;
	char msg[5][50];
	count = *((int *) ((char *) call_args + 0));
	for (size_t i1 = 0; i1 < (5); ++i1)
		for (size_t i2 = 0; i2 < (50); ++i2)
			msg[i1][i2] = *((char *) ((char *) call_args + 0 + (sizeof(int))) + (i1) * ((sizeof(char)) * (50)) + (i2) * (sizeof(char)));
	int ret_val;
	ret_val = print_messages(count, msg);
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
