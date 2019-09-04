#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>
#include "ocalls_builder.h"
#include "ocalls_reader.h"
#define c_vec_len(V) (sizeof(V)/sizeof((V)[0]))
const unsigned long __function_print_int = 2;
int print_int(int msg) {
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	int __flatcc_reference_msg;
	__flatcc_reference_msg = msg;
	__ocall_wrapper_print_int_start_as_root(&builder);
	__ocall_wrapper_print_int_msg_add(&builder, __flatcc_reference_msg);
	__ocall_wrapper_print_int_end_as_root(&builder);
	void* buf;
	size_t size;
	buf = (void *) flatcc_builder_finalize_buffer(&builder, &size);
	char return_address[1024];
	ocall(__function_print_int, buf, size, return_address, 1024);
	free(buf);
	flatcc_builder_clear(&builder);
	int __return_value;
	__return_value = __ocall_wrapper_print_int___return_value(__ocall_wrapper_print_int_as_root(return_address));
	return __return_value;
}
const unsigned long __function_print_string = 1;
int print_string(signed char msg[50]) {
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	flatbuffers_int8_vec_ref_t __flatcc_reference_msg;
	signed char __data_0[1 * (50)];
	for (size_t i1 = 0; i1 < (50); ++i1) {
		__data_0[i1 + (50) * (0)] = msg[i1];
	}	__flatcc_reference_msg = flatbuffers_int8_vec_create(&builder, __data_0, c_vec_len(__data_0));
	__ocall_wrapper_print_string_start_as_root(&builder);
	__ocall_wrapper_print_string_msg_add(&builder, __flatcc_reference_msg);
	__ocall_wrapper_print_string_end_as_root(&builder);
	void* buf;
	size_t size;
	buf = (void *) flatcc_builder_finalize_buffer(&builder, &size);
	char return_address[1024];
	ocall(__function_print_string, buf, size, return_address, 1024);
	free(buf);
	flatcc_builder_clear(&builder);
	int __return_value;
	__return_value = __ocall_wrapper_print_string___return_value(__ocall_wrapper_print_string_as_root(return_address));
	return __return_value;
}
