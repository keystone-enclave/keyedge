#include <keystone.h>
#include <edge_call.h>
#include "ocalls.h"
#include "ocalls_common.h"
#include "ocalls_builder.h"
#include "ocalls_reader.h"
void __wrapper_print_return_value(void* buffer) {
	struct edge_call* edge_call = (struct edge_call*) buffer;
	uintptr_t call_args;
	size_t arg_len;
	if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;
		return;
	}
	int msg;
	__ocall_wrapper_print_return_value_table_t function_reference = __ocall_wrapper_print_return_value_as_root((void *) call_args);
	int __flatcc_reference_msg = __ocall_wrapper_print_return_value_msg(function_reference);
	{
		msg = __flatcc_reference_msg;
	}
	int ret_val;
	ret_val = print_return_value(msg);
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	__ocall_wrapper_print_return_value_start_as_root(&builder);
	int __flatcc_reference_ret_val;
	{
		__flatcc_reference_ret_val = ret_val;
	}
	__ocall_wrapper_print_return_value___return_value_add(&builder, __flatcc_reference_ret_val);
	__ocall_wrapper_print_return_value_end_as_root(&builder);
	void* __buf;
	size_t __size;
	__buf = (void *) flatcc_builder_finalize_buffer(&builder, &__size);
	uintptr_t data_section = edge_call_data_ptr();
	memcpy((void *) data_section, __buf, __size);
	free(__buf);
	flatcc_builder_clear(&builder);
	if (edge_call_setup_ret(edge_call, (void*) data_section, 1024)) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_PTR;
	} else {
		edge_call -> return_data.call_status = CALL_STATUS_OK;
	}
	return;
}
void __wrapper_print_string(void* buffer) {
	struct edge_call* edge_call = (struct edge_call*) buffer;
	uintptr_t call_args;
	size_t arg_len;
	if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;
		return;
	}
	int *msg;
	__ocall_wrapper_print_string_table_t function_reference = __ocall_wrapper_print_string_as_root((void *) call_args);
	__pointer_flatbuffers_int32_table_t __flatcc_reference_msg = __ocall_wrapper_print_string_msg(function_reference);
	{
		if (__pointer_flatbuffers_int32___is_null(__flatcc_reference_msg)) {
			msg = 0;
		} else {
			msg = (int *) malloc(sizeof(int));
			{
				(*msg) = __pointer_flatbuffers_int32___data(__flatcc_reference_msg);
			}
		}
	}
	int ret_val;
	ret_val = print_string(msg);
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	__ocall_wrapper_print_string_start_as_root(&builder);
	int __flatcc_reference_ret_val;
	{
		__flatcc_reference_ret_val = ret_val;
	}
	__ocall_wrapper_print_string___return_value_add(&builder, __flatcc_reference_ret_val);
	__ocall_wrapper_print_string_end_as_root(&builder);
	void* __buf;
	size_t __size;
	__buf = (void *) flatcc_builder_finalize_buffer(&builder, &__size);
	uintptr_t data_section = edge_call_data_ptr();
	memcpy((void *) data_section, __buf, __size);
	free(__buf);
	flatcc_builder_clear(&builder);
	if (edge_call_setup_ret(edge_call, (void*) data_section, 1024)) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_PTR;
	} else {
		edge_call -> return_data.call_status = CALL_STATUS_OK;
	}
	return;
}
void register_functions() {
	register_call(__function_print_return_value, __wrapper_print_return_value);
	register_call(__function_print_string, __wrapper_print_string);
	return;
}