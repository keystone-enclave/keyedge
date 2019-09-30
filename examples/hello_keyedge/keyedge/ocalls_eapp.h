#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>
#include "ocalls.h"
#include "ocalls_common.h"
#include "ocalls_builder.h"
#include "ocalls_reader.h"
int print_return_value(int msg) {
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	int __0_msg;
	{
		__0_msg = msg;
	}
	__ocall_wrapper_print_return_value_start_as_root(&builder);
	__ocall_wrapper_print_return_value_msg_add(&builder, __0_msg);
	__ocall_wrapper_print_return_value_end_as_root(&builder);
	void* __buf;
	size_t __size;
	__buf = (void *) flatcc_builder_finalize_buffer(&builder, &__size);
	char return_address[1024];
	ocall(__function_print_return_value, __buf, __size, return_address, 1024);
	free(__buf);
	flatcc_builder_clear(&builder);
	int __return_value;
	{
		__return_value = __ocall_wrapper_print_return_value___return_value(__ocall_wrapper_print_return_value_as_root(return_address));
	};
	return __return_value;
}
int print_string(char *msg) {
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	size_t __0_msg_keyedge_size = strlen(msg) + 1;
	flatbuffers_int8_vec_ref_t __0_msg;
	{
		char* __keyedge_array_data_2 = (char*) malloc(1 * (__0_msg_keyedge_size));
		for (size_t i2_0 = 0; i2_0 < (size_t) (__0_msg_keyedge_size); ++i2_0) {
			{
				__keyedge_array_data_2[i2_0 + (__0_msg_keyedge_size) * (0)] = ( msg)[i2_0];
			}
		}
		__0_msg = flatbuffers_int8_vec_create(&builder, (signed char*) __keyedge_array_data_2, 1 * (__0_msg_keyedge_size));
		free(__keyedge_array_data_2);
	}
	__ocall_wrapper_print_string_start_as_root(&builder);
	__ocall_wrapper_print_string_msg_add(&builder, __0_msg);
	__ocall_wrapper_print_string_end_as_root(&builder);
	void* __buf;
	size_t __size;
	__buf = (void *) flatcc_builder_finalize_buffer(&builder, &__size);
	char return_address[1024];
	ocall(__function_print_string, __buf, __size, return_address, 1024);
	free(__buf);
	flatcc_builder_clear(&builder);
	int __return_value;
	{
		__return_value = __ocall_wrapper_print_string___return_value(__ocall_wrapper_print_string_as_root(return_address));
	};
	return __return_value;
}
