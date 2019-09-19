#ifndef __OCALLS_HEADER_H
#define __OCALLS_HEADER_H

#ifdef __KEYEDGE
#define keyedge_vla __attribute__((annotate("KEYEDGE_VLA")))
#define keyedge_size __attribute__((annotate("KEYEDGE_SIZE")))
#define keyedge_str __attribute__((annotate("KEYEDGE_VLA"))) __attribute__((annotate("KEYEDGE_STR")))

#else
#define keyedge_vla
#define keyedge_size
#define keyedge_str

#endif

#endif

