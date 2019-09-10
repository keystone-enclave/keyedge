#include <keystone.h>
#include <edge_call.h>
#include "ocalls_builder.h"
#include "ocalls_reader.h"
int print_pointer(signed char name, int *value);
void __wrapper_print_pointer(void* buffer) {
	struct edge_call* edge_call = (struct edge_call*) buffer;
	uintptr_t call_args;
	size_t arg_len;
	if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;
		return;
	}
	signed char name;
	int *value;
	__ocall_wrapper_print_pointer_table_t function_reference = __ocall_wrapper_print_pointer_as_root((void *) call_args);
	signed char __flatcc_reference_name = __ocall_wrapper_print_pointer_name(function_reference);
	{
		name = __flatcc_reference_name;
	}
	__pointer_int_table_t __flatcc_reference_value = __ocall_wrapper_print_pointer_value(function_reference);
	{
		if (__pointer_int___is_null(__flatcc_reference_value)) {
			value = 0;
		} else {
			value = (int *) malloc(sizeof(int));
			{
				(*value) = __pointer_int___data(__flatcc_reference_value);
			}
		}
	}
	int ret_val;
	ret_val = print_pointer(name, value);
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	int __flatcc_reference_ret_val;
	{
		__flatcc_reference_ret_val = ret_val;
	}
	__ocall_wrapper_print_pointer_start_as_root(&builder);
	__ocall_wrapper_print_pointer___return_value_add(&builder, __flatcc_reference_ret_val);
	__ocall_wrapper_print_pointer_end_as_root(&builder);
	void* buf;
	size_t size;
	buf = (void *) flatcc_builder_finalize_buffer(&builder, &size);
	uintptr_t data_section = edge_call_data_ptr();
	memcpy((void *) data_section, buf, size);
	free(buf);
	flatcc_builder_clear(&builder);
	if (edge_call_setup_ret(edge_call, (void*) data_section, 1024)) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_PTR;
	} else {
		edge_call -> return_data.call_status = CALL_STATUS_OK;
	}
	return;
}
void register_functions() {
	const unsigned long __function_print_pointer = 1;
	register_call(__function_print_pointer, __wrapper_print_pointer);
	return;
}
