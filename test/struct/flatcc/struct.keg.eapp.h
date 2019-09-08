#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>
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
const unsigned long __function_print_messages = 1;
int print_messages(int count, message m[5]) {
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	int __flatcc_reference_count;
	{
		__flatcc_reference_count = count;
	}
	message_vec_ref_t __flatcc_reference_m;
	{
		message_ref_t __keyedge_array_data_1[1 * (5)];
		for (size_t i1 = 0; i1 < (5); ++i1) {
			{
				message_start(&builder);
				tim_ref_t __3_t;
				{
					tim_start(&builder);
					int __4_h;
					{
						__4_h = m[i1].t.h;
					}
					int __4_m;
					{
						__4_m = m[i1].t.m;
					}
					int __4_s;
					{
						__4_s = m[i1].t.s;
					}
					tim_h_add(&builder, __4_h);
					tim_m_add(&builder, __4_m);
					tim_s_add(&builder, __4_s);
					__3_t = tim_end(&builder);
				}
				flatbuffers_int8_vec_ref_t __3_sender;
				{
					signed char __keyedge_array_data_4[1 * (10)];
					for (size_t i4 = 0; i4 < (10); ++i4) {
						{
							__keyedge_array_data_4[i4 + (10) * (0)] = m[i1].sender[i4];
						}
					}
					__3_sender = flatbuffers_int8_vec_create(&builder, __keyedge_array_data_4, 1 * (10));
				}
				flatbuffers_int8_vec_ref_t __3_content;
				{
					signed char __keyedge_array_data_4[1 * (10)];
					for (size_t i4 = 0; i4 < (10); ++i4) {
						{
							__keyedge_array_data_4[i4 + (10) * (0)] = m[i1].content[i4];
						}
					}
					__3_content = flatbuffers_int8_vec_create(&builder, __keyedge_array_data_4, 1 * (10));
				}
				message_t_add(&builder, __3_t);
				message_sender_add(&builder, __3_sender);
				message_content_add(&builder, __3_content);
				__keyedge_array_data_1[i1 + (5) * (0)] = message_end(&builder);
			}
		}
		__flatcc_reference_m = message_vec_create(&builder, __keyedge_array_data_1, 1 * (5));
	}
	__ocall_wrapper_print_messages_start_as_root(&builder);
	__ocall_wrapper_print_messages_count_add(&builder, __flatcc_reference_count);
	__ocall_wrapper_print_messages_m_add(&builder, __flatcc_reference_m);
	__ocall_wrapper_print_messages_end_as_root(&builder);
	void* buf;
	size_t size;
	buf = (void *) flatcc_builder_finalize_buffer(&builder, &size);
	char return_address[1024];
	ocall(__function_print_messages, buf, size, return_address, 1024);
	free(buf);
	flatcc_builder_clear(&builder);
	int __return_value;
	{
		__return_value = __ocall_wrapper_print_messages___return_value(__ocall_wrapper_print_messages_as_root(return_address));
	}
	return __return_value;
}
