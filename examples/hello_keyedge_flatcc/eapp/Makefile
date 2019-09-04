CC = riscv64-unknown-linux-gnu-gcc
LD = riscv64-unknown-linux-gnu-ld

SDK_LIB_DIR = $(KEYSTONE_SDK_DIR)/lib
KEYEDGE_LIB_DIR = ../../../lib

SDK_INCLUDE_APP_DIR = $(SDK_LIB_DIR)/app/include
SDK_INCLUDE_EDGE_DIR = $(SDK_LIB_DIR)/edge/include
KEYEDGE_INCLUDE_DIR = ../../../include
KEYEDGE_INCLUDE_FLATCC_CUSTOM_DIR = ../flatcc

CFLAGS += -I$(SDK_INCLUDE_APP_DIR) -I$(SDK_INCLUDE_EDGE_DIR) -I$(KEYEDGE_INCLUDE_DIR) -I$(KEYEDGE_INCLUDE_FLATCC_CUSTOM_DIR) -Wall -Werror
LDFLAGS = -static -L$(SDK_LIB_DIR) -L$(KEYEDGE_LIB_DIR)

EAPP = eapp_hello_keyedge_flatcc
OBJS = $(EAPP).o

all: $(EAPP)

$(EAPP): $(OBJS)
	$(LD) $(LDFLAGS) \
	-o $(EAPP) \
	$(OBJS) \
	$(SDK_LIB_DIR)/libkeystone-eapp.a \
	$(SDK_LIB_DIR)/libkeystone-edge.a \
	$(KEYEDGE_LIB_DIR)/flatcc/flatccrt.a
	chmod -x $@

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(APP_BIN) $(EXTRA_CLEAN)
