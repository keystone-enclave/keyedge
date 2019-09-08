#ifndef OCALLS_READER_H
#define OCALLS_READER_H

/* Generated by flatcc 0.6.1-dev FlatBuffers schema compiler for C by dvide.com */

#ifndef FLATBUFFERS_COMMON_READER_H
#include "flatbuffers_common_reader.h"
#endif
#include "flatcc/flatcc_flatbuffers.h"
#ifndef __alignas_is_defined
#include <stdalign.h>
#endif
#include "flatcc/flatcc_prologue.h"
#ifndef flatbuffers_identifier
#define flatbuffers_identifier 0
#endif
#ifndef flatbuffers_extension
#define flatbuffers_extension ".bin"
#endif


typedef const struct tim_table *tim_table_t;
typedef struct tim_table *tim_mutable_table_t;
typedef const flatbuffers_uoffset_t *tim_vec_t;
typedef flatbuffers_uoffset_t *tim_mutable_vec_t;
typedef const struct message_table *message_table_t;
typedef struct message_table *message_mutable_table_t;
typedef const flatbuffers_uoffset_t *message_vec_t;
typedef flatbuffers_uoffset_t *message_mutable_vec_t;
typedef const struct __ocall_wrapper_print_messages_table *__ocall_wrapper_print_messages_table_t;
typedef struct __ocall_wrapper_print_messages_table *__ocall_wrapper_print_messages_mutable_table_t;
typedef const flatbuffers_uoffset_t *__ocall_wrapper_print_messages_vec_t;
typedef flatbuffers_uoffset_t *__ocall_wrapper_print_messages_mutable_vec_t;
#ifndef tim_identifier
#define tim_identifier flatbuffers_identifier
#endif
#define tim_type_hash ((flatbuffers_thash_t)0xc00af369)
#define tim_type_identifier "\x69\xf3\x0a\xc0"
#ifndef message_identifier
#define message_identifier flatbuffers_identifier
#endif
#define message_type_hash ((flatbuffers_thash_t)0x24f208e4)
#define message_type_identifier "\xe4\x08\xf2\x24"
#ifndef __ocall_wrapper_print_messages_identifier
#define __ocall_wrapper_print_messages_identifier flatbuffers_identifier
#endif
#define __ocall_wrapper_print_messages_type_hash ((flatbuffers_thash_t)0x93fec525)
#define __ocall_wrapper_print_messages_type_identifier "\x25\xc5\xfe\x93"



struct tim_table { uint8_t unused__; };

static inline size_t tim_vec_len(tim_vec_t vec)
__flatbuffers_vec_len(vec)
static inline tim_table_t tim_vec_at(tim_vec_t vec, size_t i)
__flatbuffers_offset_vec_at(tim_table_t, vec, i, 0)
__flatbuffers_table_as_root(tim)

__flatbuffers_define_scalar_field(0, tim, h, flatbuffers_int64, int64_t, INT64_C(0))
__flatbuffers_define_scalar_field(1, tim, m, flatbuffers_int64, int64_t, INT64_C(0))
__flatbuffers_define_scalar_field(2, tim, s, flatbuffers_int64, int64_t, INT64_C(0))

struct message_table { uint8_t unused__; };

static inline size_t message_vec_len(message_vec_t vec)
__flatbuffers_vec_len(vec)
static inline message_table_t message_vec_at(message_vec_t vec, size_t i)
__flatbuffers_offset_vec_at(message_table_t, vec, i, 0)
__flatbuffers_table_as_root(message)

__flatbuffers_define_table_field(0, message, t, tim_table_t, 0)
__flatbuffers_define_vector_field(1, message, sender, flatbuffers_int8_vec_t, 0)
__flatbuffers_define_vector_field(2, message, content, flatbuffers_int8_vec_t, 0)

struct __ocall_wrapper_print_messages_table { uint8_t unused__; };

static inline size_t __ocall_wrapper_print_messages_vec_len(__ocall_wrapper_print_messages_vec_t vec)
__flatbuffers_vec_len(vec)
static inline __ocall_wrapper_print_messages_table_t __ocall_wrapper_print_messages_vec_at(__ocall_wrapper_print_messages_vec_t vec, size_t i)
__flatbuffers_offset_vec_at(__ocall_wrapper_print_messages_table_t, vec, i, 0)
__flatbuffers_table_as_root(__ocall_wrapper_print_messages)

__flatbuffers_define_scalar_field(0, __ocall_wrapper_print_messages, count, flatbuffers_int64, int64_t, INT64_C(0))
__flatbuffers_define_vector_field(1, __ocall_wrapper_print_messages, m, message_vec_t, 0)
__flatbuffers_define_scalar_field(2, __ocall_wrapper_print_messages, __return_value, flatbuffers_int64, int64_t, INT64_C(0))


#include "flatcc/flatcc_epilogue.h"
#endif /* OCALLS_READER_H */
