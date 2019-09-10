#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>
#include "ocalls_builder.h"
#include "ocalls_reader.h"
const unsigned long __function_print_pointer = 1;
int print_pointer(signed char name, int *value) {
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	signed char __flatcc_reference_name;
	{
		__flatcc_reference_name = name;
	}
	int __flatcc_reference_value;
	{
		if (value) {
			{
				__flatcc_reference_value = (*value);
			}
		}
	}
	__ocall_wrapper_print_pointer_start_as_root(&builder);
	__ocall_wrapper_print_pointer_name_add(&builder, __flatcc_reference_name);
	__ocall_wrapper_print_pointer_value_add(&builder, __flatcc_reference_value);
	__ocall_wrapper_print_pointer_end_as_root(&builder);
	void* buf;
	size_t size;
	buf = (void *) flatcc_builder_finalize_buffer(&builder, &size);
	char return_address[1024];
	ocall(__function_print_pointer, buf, size, return_address, 1024);
	free(buf);
	flatcc_builder_clear(&builder);
	int __return_value;
	{
		__return_value = __ocall_wrapper_print_pointer___return_value(__ocall_wrapper_print_pointer_as_root(return_address));
	}
	return __return_value;
}
