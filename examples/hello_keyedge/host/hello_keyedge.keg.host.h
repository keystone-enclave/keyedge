#include <keystone.h>
#include <edge_call.h>
int print_message(char msg[50]);
void __wrapper_print_message(void* buffer) {
	struct edge_call* edge_call = (struct edge_call*) buffer;
	uintptr_t call_args;
	size_t arg_len;
	if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;
		return;
	}
	char msg[50];
	for (size_t i1 = 0; i1 < (50); ++i1)
		msg[i1] = *((char *) ((char *) call_args + 0) + (i1) * (sizeof(char)));
	int ret_val;
	ret_val = print_message(msg);
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
	const unsigned long __function_print_message = 1;
	register_call(__function_print_message, __wrapper_print_message);
	return;
}
