#include <keystone.h>
#include <edge_call.h>
#include "ocalls.h"
#include "ocalls_common.h"
#include "ocalls_builder.h"
#include "ocalls_reader.h"
void __wrapper_test_nested_struct(void* buffer) {
	struct edge_call* edge_call = (struct edge_call*) buffer;
	uintptr_t call_args;
	size_t arg_len;
	if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;
		return;
	}
	nest1 n;
	__ocall_wrapper_test_nested_struct_table_t function_reference = __ocall_wrapper_test_nested_struct_as_root((void *) call_args);
	nest1_table_t __flatcc_reference_n = __ocall_wrapper_test_nested_struct_n(function_reference);
	{
		{
			if (__pointer_nest2___is_null(nest1_n(__flatcc_reference_n))) {
				n.n = 0;
			} else {
				n.n = (nest2 *) malloc(sizeof(nest2));
				{
					{
						if (__pointer_nest3___is_null(nest2_n(__pointer_nest2___data(nest1_n(__flatcc_reference_n))))) {
							(*n.n).n = 0;
						} else {
							(*n.n).n = (nest3 *) malloc(sizeof(nest3));
							{
								size_t __8_str_keyedge_size = flatbuffers_int8_vec_len(nest3_str(__pointer_nest3___data(nest2_n(__pointer_nest2___data(nest1_n(__flatcc_reference_n))))));
								{
									(*(*n.n).n).str = (char *) malloc((1 * (__8_str_keyedge_size)) * sizeof(char));
									for (size_t i9_0 = 0; i9_0 < (size_t) (__8_str_keyedge_size); ++i9_0) {
										{
											( (*(*n.n).n).str)[i9_0] = flatbuffers_int8_vec_at(nest3_str(__pointer_nest3___data(nest2_n(__pointer_nest2___data(nest1_n(__flatcc_reference_n))))), i9_0 + (__8_str_keyedge_size) * (0));
										}
									}
								}
								
							}
						}
					}
					size_t __5_str_keyedge_size = flatbuffers_int8_vec_len(nest2_str(__pointer_nest2___data(nest1_n(__flatcc_reference_n))));
					{
						(*n.n).str = (char *) malloc((1 * (__5_str_keyedge_size)) * sizeof(char));
						for (size_t i6_0 = 0; i6_0 < (size_t) (__5_str_keyedge_size); ++i6_0) {
							{
								( (*n.n).str)[i6_0] = flatbuffers_int8_vec_at(nest2_str(__pointer_nest2___data(nest1_n(__flatcc_reference_n))), i6_0 + (__5_str_keyedge_size) * (0));
							}
						}
					}
					
				}
			}
		}
		size_t __2_str_keyedge_size = flatbuffers_int8_vec_len(nest1_str(__flatcc_reference_n));
		{
			n.str = (char *) malloc((1 * (__2_str_keyedge_size)) * sizeof(char));
			for (size_t i3_0 = 0; i3_0 < (size_t) (__2_str_keyedge_size); ++i3_0) {
				{
					( n.str)[i3_0] = flatbuffers_int8_vec_at(nest1_str(__flatcc_reference_n), i3_0 + (__2_str_keyedge_size) * (0));
				}
			}
		}
		
	}
	int ret_val;
	ret_val = test_nested_struct(n);
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	__ocall_wrapper_test_nested_struct_start_as_root(&builder);
	int __flatcc_reference_ret_val;
	{
		__flatcc_reference_ret_val = ret_val;
	}
	__ocall_wrapper_test_nested_struct___return_value_add(&builder, __flatcc_reference_ret_val);
	__ocall_wrapper_test_nested_struct_end_as_root(&builder);
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
void __wrapper_test_two_dimensional_array(void* buffer) {
	struct edge_call* edge_call = (struct edge_call*) buffer;
	uintptr_t call_args;
	size_t arg_len;
	if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;
		return;
	}
	int arr[2][3];
	__ocall_wrapper_test_two_dimensional_array_table_t function_reference = __ocall_wrapper_test_two_dimensional_array_as_root((void *) call_args);
	flatbuffers_int32_vec_t __flatcc_reference_arr = __ocall_wrapper_test_two_dimensional_array_arr(function_reference);
	{
		
		for (size_t i2_0 = 0; i2_0 < (size_t) (3); ++i2_0) for (size_t i2_1 = 0; i2_1 < (size_t) (2); ++i2_1) {
			{
				( arr)[i2_1][i2_0] = flatbuffers_int32_vec_at(__flatcc_reference_arr, i2_0 + (3) * (i2_1 + (2) * (0)));
			}
		}
	}
	int ret_val;
	ret_val = test_two_dimensional_array(arr);
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	__ocall_wrapper_test_two_dimensional_array_start_as_root(&builder);
	int __flatcc_reference_ret_val;
	{
		__flatcc_reference_ret_val = ret_val;
	}
	__ocall_wrapper_test_two_dimensional_array___return_value_add(&builder, __flatcc_reference_ret_val);
	__ocall_wrapper_test_two_dimensional_array_end_as_root(&builder);
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
	register_call(__function_test_nested_struct, __wrapper_test_nested_struct);
	register_call(__function_test_two_dimensional_array, __wrapper_test_two_dimensional_array);
	return;
}