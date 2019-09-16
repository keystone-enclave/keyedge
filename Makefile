BIN_DIR = bin
LIB_DIR = lib
SRC_DIR = src

LIBCLANG_INCLUDE_DIR = $$HOME/llvm-project/clang/include
LIBCLANG_LIB_DIR = $$HOME/llvm-project/build/lib

CC = g++
CC_OPTION = -std=c++14 -g -Wall -I $(LIBCLANG_INCLUDE_DIR)
LD_OPTION = -L $(LIBCLANG_LIB_DIR) -lclang -Wl,-rpath=$(LIBCLANG_LIB_DIR)

KEYEDGE_OBJECTS = parser.o emitter.o main.o
KEYEDGE_LIST = $(addprefix $(BIN_DIR)/, $(KEYEDGE_OBJECTS))

RISCV_CC = riscv64-unknown-linux-gnu-gcc
FLATCC_CC_OPTION = -I$(FLATCC_DIR)/include -Wall -Werror
FLATCC_OBJECTS = builder.o emitter.o refmap.o verifier.o
FLATCC_LIST = $(addprefix $(LIB_DIR)/, $(FLATCC_OBJECTS))

all : directory keyedge flatcc

directory :
	mkdir -p $(BIN_DIR)
	mkdir -p $(LIB_DIR)
	
keyedge : $(BIN_DIR)/keyedge

$(BIN_DIR)/keyedge : $(KEYEDGE_LIST)
	$(CC) $(KEYEDGE_LIST) -o $@ $(LD_OPTION)

$(BIN_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CC_OPTION) -c $< -o $@

flatcc : $(LIB_DIR)/flatccrt.a

$(LIB_DIR)/flatccrt.a : $(FLATCC_LIST)
	ar ru $(LIB_DIR)/flatccrt.a $(FLATCC_LIST)
	ranlib $(LIB_DIR)/flatccrt.a

$(LIB_DIR)/%.o : $(FLATCC_DIR)/src/runtime/%.c
	$(RISCV_CC) $(FLATCC_CC_OPTION) -c $< -o $@

clean :
	rm -rf bin
	rm -rf lib
