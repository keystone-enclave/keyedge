CC = riscv64-unknown-linux-gnu-g++
LD = riscv64-unknown-linux-gnu-ld

SDK_LIB_DIR =$(KEYSTONE_SDK_DIR)/lib
KEYEDGE_LIB_DIR = ../../../lib

SDK_INCLUDE_HOST_DIR = $(SDK_LIB_DIR)/host/include
SDK_INCLUDE_EDGE_DIR = $(SDK_LIB_DIR)/edge/include
KEYEDGE_INCLUDE_DIR = ../../../include
KEYEDGE_INCLUDE_FLATCC_CUSTOM_DIR = ../flatcc

CCFLAGS = -I$(SDK_INCLUDE_HOST_DIR) -I$(SDK_INCLUDE_EDGE_DIR) -I$(KEYEDGE_INCLUDE_DIR) -I$(KEYEDGE_INCLUDE_FLATCC_CUSTOM_DIR) -std=c++11 -g -Wall -Werror
LDFLAGS = -L$(SDK_LIB_DIR) -L$(KEYEDGE_LIB_DIR)

HOST = host_pointer
OBJS = $(HOST).o

all: $(HOST)

$(HOST): $(OBJS)
	$(CC) $(LDFLAGS) $(CCFLAGS) \
	-o $(HOST) \
	$(OBJS) \
	$(SDK_LIB_DIR)/libkeystone-host.a \
	$(SDK_LIB_DIR)/libkeystone-edge.a \
	$(KEYEDGE_LIB_DIR)/flatcc/flatccrt.a
	
$(OBJS): %.o: %.cpp
	$(CC) $(CCFLAGS) -c $<
	
clean:
	rm -f *.o $(HOST)
