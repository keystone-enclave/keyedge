#ifndef __OCALLS_HEADER_H
#define __OCALLS_HEADER_H

#ifdef __KEYEDGE
#define keyedge_vla __attribute__((annotate("KEYEDGE_VLA")))
#define keyedge_size __attribute__((annotate("KEYEDGE_SIZE")))
#define keyedge_str __attribute__((annotate("KEYEDGE_VLA"))) __attribute__((annotate("KEYEDGE_STR")))
#define keyedge_buffer(x) __attribute__((annotate("KEYEDGE_BUFFER_" #x)))
#define keyedge_inverse __attribute__((annotate("KEYEDGE_INVERSE")))

#else
#define keyedge_vla
#define keyedge_size
#define keyedge_str
#define keyedge_buffer(x)
#define keyedge_inverse

#endif

#endif

