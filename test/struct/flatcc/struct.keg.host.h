#include <keystone.h>
#include <edge_call.h>
#include "ocalls_builder.h"
#include "ocalls_reader.h"
typedef struct {
	int h;
	int m;
	int s;
} tim;
typedef struct {
	tim t;
	signed char sender[10];
	signed char content[10];
} message;
int print_messages(int count, message m[5]);
void __wrapper_print_messages(void* buffer) {
	struct edge_call* edge_call = (struct edge_call*) buffer;
	uintptr_t call_args;
	size_t arg_len;
	if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
		edge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;
		return;
	}
	int count;
	message m[5];
	__ocall_wrapper_print_messages_table_t function_reference = __ocall_wrapper_print_messages_as_root((void *) call_args);
	int __flatcc_reference_count = __ocall_wrapper_print_messages_count(function_reference);
	{
		count = __flatcc_reference_count;
	}
	message_vec_t __flatcc_reference_m = __ocall_wrapper_print_messages_m(function_reference);
	{
		for (size_t i1 = 0; i1 < (5); ++i1) {
			{
				{
					{
						m[i1].t.h = tim_h(message_t(message_vec_at(__flatcc_reference_m, i1 + (5) * (0))));
					}
					{
						m[i1].t.m = tim_m(message_t(message_vec_at(__flatcc_reference_m, i1 + (5) * (0))));
					}
					{
						m[i1].t.s = tim_s(message_t(message_vec_at(__flatcc_reference_m, i1 + (5) * (0))));
					}
				}
				{
					for (size_t i4 = 0; i4 < (10); ++i4) {
						{
							m[i1].sender[i4] = flatbuffers_int8_vec_at(message_sender(message_vec_at(__flatcc_reference_m, i1 + (5) * (0))), i4 + (10) * (0));
						}
					}
				}
				{
					for (size_t i4 = 0; i4 < (10); ++i4) {
						{
							m[i1].content[i4] = flatbuffers_int8_vec_at(message_content(message_vec_at(__flatcc_reference_m, i1 + (5) * (0))), i4 + (10) * (0));
						}
					}
				}
			}
		}
	}
	int ret_val;
	ret_val = print_messages(count, m);
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	int __flatcc_reference_ret_val;
	{
		__flatcc_reference_ret_val = ret_val;
	}
	__ocall_wrapper_print_messages_start_as_root(&builder);
	__ocall_wrapper_print_messages___return_value_add(&builder, __flatcc_reference_ret_val);
	__ocall_wrapper_print_messages_end_as_root(&builder);
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
	const unsigned long __function_print_messages = 1;
	register_call(__function_print_messages, __wrapper_print_messages);
	return;
}
