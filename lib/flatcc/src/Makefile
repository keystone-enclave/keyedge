CC = riscv64-unknown-linux-gnu-gcc
LD = riscv64-unknown-linux-gnu-ld

KEYEDGE_INCLUDE_DIR = ../../../include

CFLAGS += -I$(KEYEDGE_INCLUDE_DIR) -Wall -Werror

OBJS = $(EAPP).o

flatcc.a: builder.o emitter.o refmap.o verifier.o
	ar ru flatccrt.a builder.o emitter.o refmap.o verifier.o
	ranlib flatccrt.a

builder.o: builder.c
	$(CC) $(CFLAGS) -c builder.c

emitter.o: emitter.c
	$(CC) $(CFLAGS) -c emitter.c

refmap.o: refmap.c
	$(CC) $(CFLAGS) -c refmap.c

verifier.o: verifier.c
	$(CC) $(CFLAGS) -c verifier.c

