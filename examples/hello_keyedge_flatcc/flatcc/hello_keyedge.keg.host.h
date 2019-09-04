#include <keystone.h>
#include <edge_call.h>
#include "ocalls_builder.h"
#include "ocalls_reader.h"
#define c_vec_len(V) (sizeof(V)/sizeof((V)[0]))
int print_int(int msg);
void __wrapper_print_int(void* buffer) {
	struct edge_call* edge_call = (struct edge_call*) buffer;
	uintptr_t call_args;
	size_t arg_len;
	if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;
		return;
	}
	int msg;
	__ocall_wrapper_print_int_table_t function_reference = __ocall_wrapper_print_int_as_root((void *) call_args);
	int __flatcc_reference_msg = __ocall_wrapper_print_int_msg(function_reference);
	msg = __flatcc_reference_msg;
	int ret_val;
	ret_val = print_int(msg);
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	int __flatcc_reference_ret_val;
	__flatcc_reference_ret_val = ret_val;
	__ocall_wrapper_print_int_start_as_root(&builder);
	__ocall_wrapper_print_int___return_value_add(&builder, __flatcc_reference_ret_val);
	__ocall_wrapper_print_int_end_as_root(&builder);
	void* buf;
	size_t size;
	buf = (void *) flatcc_builder_finalize_buffer(&builder, &size);
	uintptr_t data_section = edge_call_data_ptr();
	memcpy((void *) data_section, buf, size);
	if (edge_call_setup_ret(edge_call, (void*) data_section, 1024)) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_PTR;
	} else {
		edge_call -> return_data.call_status = CALL_STATUS_OK;
	}
	return;
}
int print_string(signed char msg[50]);
void __wrapper_print_string(void* buffer) {
	struct edge_call* edge_call = (struct edge_call*) buffer;
	uintptr_t call_args;
	size_t arg_len;
	if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;
		return;
	}
	signed char msg[50];
	__ocall_wrapper_print_string_table_t function_reference = __ocall_wrapper_print_string_as_root((void *) call_args);
	flatbuffers_int8_vec_t __flatcc_reference_msg = __ocall_wrapper_print_string_msg(function_reference);
	for (size_t i1 = 0; i1 < (50); ++i1) {
		msg[i1] = flatbuffers_int8_vec_at(__flatcc_reference_msg, i1 + (50) * (0));
	}	int ret_val;
	ret_val = print_string(msg);
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	int __flatcc_reference_ret_val;
	__flatcc_reference_ret_val = ret_val;
	__ocall_wrapper_print_string_start_as_root(&builder);
	__ocall_wrapper_print_string___return_value_add(&builder, __flatcc_reference_ret_val);
	__ocall_wrapper_print_string_end_as_root(&builder);
	void* buf;
	size_t size;
	buf = (void *) flatcc_builder_finalize_buffer(&builder, &size);
	uintptr_t data_section = edge_call_data_ptr();
	memcpy((void *) data_section, buf, size);
	if (edge_call_setup_ret(edge_call, (void*) data_section, 1024)) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_PTR;
	} else {
		edge_call -> return_data.call_status = CALL_STATUS_OK;
	}
	return;
}
void register_functions() {
	const unsigned long __function_print_int = 2;
	register_call(__function_print_int, __wrapper_print_int);
	const unsigned long __function_print_string = 1;
	register_call(__function_print_string, __wrapper_print_string);
	return;
}
