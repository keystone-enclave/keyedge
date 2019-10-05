#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>
#include "ocalls.h"
#include "ocalls_common.h"
#include "ocalls_builder.h"
#include "ocalls_reader.h"
int test_nested_struct(nest1 n) {
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	nest1_ref_t __0_n;
	{
		nest1_start(&builder);
		__pointer_nest2_ref_t __2_n;
		{
			if (n.n) {
				nest2_ref_t __keyedge_pointer_data_3;
				{
					nest2_start(&builder);
					__pointer_nest3_ref_t __5_n;
					{
						if ((*n.n).n) {
							nest3_ref_t __keyedge_pointer_data_6;
							{
								nest3_start(&builder);
								size_t __8_str_keyedge_size = strlen((*(*n.n).n).str) + 1;
								flatbuffers_int8_vec_ref_t __8_str;
								{
									char* __keyedge_array_data_9 = (char*) malloc(1 * (__8_str_keyedge_size) * sizeof(char));
									for (size_t i9_0 = 0; i9_0 < (size_t) (__8_str_keyedge_size); ++i9_0) {
										{
											__keyedge_array_data_9[i9_0 + (__8_str_keyedge_size) * (0)] = ( (*(*n.n).n).str)[i9_0];
										}
									}
									__8_str = flatbuffers_int8_vec_create(&builder, (signed char*) __keyedge_array_data_9, 1 * (__8_str_keyedge_size));
									free(__keyedge_array_data_9);
								}
								nest3_str_add(&builder, __8_str);
								__keyedge_pointer_data_6 = nest3_end(&builder);
							}
							__5_n = __pointer_nest3_create(&builder, 0, __keyedge_pointer_data_6);
						} else {
							__pointer_nest3_start(&builder);
							__pointer_nest3___is_null_add(&builder, 1);
							__5_n = __pointer_nest3_end(&builder);
						}
					}
					size_t __5_str_keyedge_size = strlen((*n.n).str) + 1;
					flatbuffers_int8_vec_ref_t __5_str;
					{
						char* __keyedge_array_data_6 = (char*) malloc(1 * (__5_str_keyedge_size) * sizeof(char));
						for (size_t i6_0 = 0; i6_0 < (size_t) (__5_str_keyedge_size); ++i6_0) {
							{
								__keyedge_array_data_6[i6_0 + (__5_str_keyedge_size) * (0)] = ( (*n.n).str)[i6_0];
							}
						}
						__5_str = flatbuffers_int8_vec_create(&builder, (signed char*) __keyedge_array_data_6, 1 * (__5_str_keyedge_size));
						free(__keyedge_array_data_6);
					}
					nest2_n_add(&builder, __5_n);
					nest2_str_add(&builder, __5_str);
					__keyedge_pointer_data_3 = nest2_end(&builder);
				}
				__2_n = __pointer_nest2_create(&builder, 0, __keyedge_pointer_data_3);
			} else {
				__pointer_nest2_start(&builder);
				__pointer_nest2___is_null_add(&builder, 1);
				__2_n = __pointer_nest2_end(&builder);
			}
		}
		size_t __2_str_keyedge_size = strlen(n.str) + 1;
		flatbuffers_int8_vec_ref_t __2_str;
		{
			char* __keyedge_array_data_3 = (char*) malloc(1 * (__2_str_keyedge_size) * sizeof(char));
			for (size_t i3_0 = 0; i3_0 < (size_t) (__2_str_keyedge_size); ++i3_0) {
				{
					__keyedge_array_data_3[i3_0 + (__2_str_keyedge_size) * (0)] = ( n.str)[i3_0];
				}
			}
			__2_str = flatbuffers_int8_vec_create(&builder, (signed char*) __keyedge_array_data_3, 1 * (__2_str_keyedge_size));
			free(__keyedge_array_data_3);
		}
		nest1_n_add(&builder, __2_n);
		nest1_str_add(&builder, __2_str);
		__0_n = nest1_end(&builder);
	}
	__ocall_wrapper_test_nested_struct_start_as_root(&builder);
	__ocall_wrapper_test_nested_struct_n_add(&builder, __0_n);
	__ocall_wrapper_test_nested_struct_end_as_root(&builder);
	void* __buf;
	size_t __size;
	__buf = (void *) flatcc_builder_finalize_buffer(&builder, &__size);
	char return_address[1024];
	ocall(__function_test_nested_struct, __buf, __size, return_address, 1024);
	free(__buf);
	flatcc_builder_clear(&builder);
	int __return_value;
	{
		__return_value = __ocall_wrapper_test_nested_struct___return_value(__ocall_wrapper_test_nested_struct_as_root(return_address));
	};
	return __return_value;
}
int test_two_dimensional_array(int arr[2][3]) {
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	flatbuffers_int32_vec_ref_t __0_arr;
	{
		int* __keyedge_array_data_2 = (int*) malloc(1 * (3) * (2) * sizeof(int));
		for (size_t i2_0 = 0; i2_0 < (size_t) (3); ++i2_0) for (size_t i2_1 = 0; i2_1 < (size_t) (2); ++i2_1) {
			{
				__keyedge_array_data_2[i2_0 + (3) * (i2_1 + (2) * (0))] = ( arr)[i2_1][i2_0];
			}
		}
		__0_arr = flatbuffers_int32_vec_create(&builder,  __keyedge_array_data_2, 1 * (3) * (2));
		free(__keyedge_array_data_2);
	}
	__ocall_wrapper_test_two_dimensional_array_start_as_root(&builder);
	__ocall_wrapper_test_two_dimensional_array_arr_add(&builder, __0_arr);
	__ocall_wrapper_test_two_dimensional_array_end_as_root(&builder);
	void* __buf;
	size_t __size;
	__buf = (void *) flatcc_builder_finalize_buffer(&builder, &__size);
	char return_address[1024];
	ocall(__function_test_two_dimensional_array, __buf, __size, return_address, 1024);
	free(__buf);
	flatcc_builder_clear(&builder);
	int __return_value;
	{
		__return_value = __ocall_wrapper_test_two_dimensional_array___return_value(__ocall_wrapper_test_two_dimensional_array_as_root(return_address));
	};
	return __return_value;
}
