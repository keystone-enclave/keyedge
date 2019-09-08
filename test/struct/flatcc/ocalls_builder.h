#ifndef OCALLS_BUILDER_H
#define OCALLS_BUILDER_H

/* Generated by flatcc 0.6.1-dev FlatBuffers schema compiler for C by dvide.com */

#ifndef OCALLS_READER_H
#include "ocalls_reader.h"
#endif
#ifndef FLATBUFFERS_COMMON_BUILDER_H
#include "flatbuffers_common_builder.h"
#endif
#include "flatcc/flatcc_prologue.h"
#ifndef flatbuffers_identifier
#define flatbuffers_identifier 0
#endif
#ifndef flatbuffers_extension
#define flatbuffers_extension ".bin"
#endif

static const flatbuffers_voffset_t __tim_required[] = { 0 };
typedef flatbuffers_ref_t tim_ref_t;
static tim_ref_t tim_clone(flatbuffers_builder_t *B, tim_table_t t);
__flatbuffers_build_table(flatbuffers_, tim, 3)

static const flatbuffers_voffset_t __message_required[] = { 0 };
typedef flatbuffers_ref_t message_ref_t;
static message_ref_t message_clone(flatbuffers_builder_t *B, message_table_t t);
__flatbuffers_build_table(flatbuffers_, message, 3)

static const flatbuffers_voffset_t ____ocall_wrapper_print_messages_required[] = { 0 };
typedef flatbuffers_ref_t __ocall_wrapper_print_messages_ref_t;
static __ocall_wrapper_print_messages_ref_t __ocall_wrapper_print_messages_clone(flatbuffers_builder_t *B, __ocall_wrapper_print_messages_table_t t);
__flatbuffers_build_table(flatbuffers_, __ocall_wrapper_print_messages, 3)

#define __tim_formal_args , int64_t v0, int64_t v1, int64_t v2
#define __tim_call_args , v0, v1, v2
static inline tim_ref_t tim_create(flatbuffers_builder_t *B __tim_formal_args);
__flatbuffers_build_table_prolog(flatbuffers_, tim, tim_identifier, tim_type_identifier)

#define __message_formal_args , tim_ref_t v0, flatbuffers_int8_vec_ref_t v1, flatbuffers_int8_vec_ref_t v2
#define __message_call_args , v0, v1, v2
static inline message_ref_t message_create(flatbuffers_builder_t *B __message_formal_args);
__flatbuffers_build_table_prolog(flatbuffers_, message, message_identifier, message_type_identifier)

#define ____ocall_wrapper_print_messages_formal_args , int64_t v0, message_vec_ref_t v1, int64_t v2
#define ____ocall_wrapper_print_messages_call_args , v0, v1, v2
static inline __ocall_wrapper_print_messages_ref_t __ocall_wrapper_print_messages_create(flatbuffers_builder_t *B ____ocall_wrapper_print_messages_formal_args);
__flatbuffers_build_table_prolog(flatbuffers_, __ocall_wrapper_print_messages, __ocall_wrapper_print_messages_identifier, __ocall_wrapper_print_messages_type_identifier)

__flatbuffers_build_scalar_field(0, flatbuffers_, tim_h, flatbuffers_int64, int64_t, 8, 8, INT64_C(0), tim)
__flatbuffers_build_scalar_field(1, flatbuffers_, tim_m, flatbuffers_int64, int64_t, 8, 8, INT64_C(0), tim)
__flatbuffers_build_scalar_field(2, flatbuffers_, tim_s, flatbuffers_int64, int64_t, 8, 8, INT64_C(0), tim)

static inline tim_ref_t tim_create(flatbuffers_builder_t *B __tim_formal_args)
{
    if (tim_start(B)
        || tim_h_add(B, v0)
        || tim_m_add(B, v1)
        || tim_s_add(B, v2)) {
        return 0;
    }
    return tim_end(B);
}

static tim_ref_t tim_clone(flatbuffers_builder_t *B, tim_table_t t)
{
    __flatbuffers_memoize_begin(B, t);
    if (tim_start(B)
        || tim_h_pick(B, t)
        || tim_m_pick(B, t)
        || tim_s_pick(B, t)) {
        return 0;
    }
    __flatbuffers_memoize_end(B, t, tim_end(B));
}

__flatbuffers_build_table_field(0, flatbuffers_, message_t, tim, message)
__flatbuffers_build_vector_field(1, flatbuffers_, message_sender, flatbuffers_int8, int8_t, message)
__flatbuffers_build_vector_field(2, flatbuffers_, message_content, flatbuffers_int8, int8_t, message)

static inline message_ref_t message_create(flatbuffers_builder_t *B __message_formal_args)
{
    if (message_start(B)
        || message_t_add(B, v0)
        || message_sender_add(B, v1)
        || message_content_add(B, v2)) {
        return 0;
    }
    return message_end(B);
}

static message_ref_t message_clone(flatbuffers_builder_t *B, message_table_t t)
{
    __flatbuffers_memoize_begin(B, t);
    if (message_start(B)
        || message_t_pick(B, t)
        || message_sender_pick(B, t)
        || message_content_pick(B, t)) {
        return 0;
    }
    __flatbuffers_memoize_end(B, t, message_end(B));
}

__flatbuffers_build_scalar_field(0, flatbuffers_, __ocall_wrapper_print_messages_count, flatbuffers_int64, int64_t, 8, 8, INT64_C(0), __ocall_wrapper_print_messages)
__flatbuffers_build_table_vector_field(1, flatbuffers_, __ocall_wrapper_print_messages_m, message, __ocall_wrapper_print_messages)
__flatbuffers_build_scalar_field(2, flatbuffers_, __ocall_wrapper_print_messages___return_value, flatbuffers_int64, int64_t, 8, 8, INT64_C(0), __ocall_wrapper_print_messages)

static inline __ocall_wrapper_print_messages_ref_t __ocall_wrapper_print_messages_create(flatbuffers_builder_t *B ____ocall_wrapper_print_messages_formal_args)
{
    if (__ocall_wrapper_print_messages_start(B)
        || __ocall_wrapper_print_messages_count_add(B, v0)
        || __ocall_wrapper_print_messages___return_value_add(B, v2)
        || __ocall_wrapper_print_messages_m_add(B, v1)) {
        return 0;
    }
    return __ocall_wrapper_print_messages_end(B);
}

static __ocall_wrapper_print_messages_ref_t __ocall_wrapper_print_messages_clone(flatbuffers_builder_t *B, __ocall_wrapper_print_messages_table_t t)
{
    __flatbuffers_memoize_begin(B, t);
    if (__ocall_wrapper_print_messages_start(B)
        || __ocall_wrapper_print_messages_count_pick(B, t)
        || __ocall_wrapper_print_messages___return_value_pick(B, t)
        || __ocall_wrapper_print_messages_m_pick(B, t)) {
        return 0;
    }
    __flatbuffers_memoize_end(B, t, __ocall_wrapper_print_messages_end(B));
}

#include "flatcc/flatcc_epilogue.h"
#endif /* OCALLS_BUILDER_H */