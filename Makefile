BIN_DIR = bin
LIB_DIR = lib
INCLUDE_DIR = include
SRC_DIR = src

LIBCLANG_INCLUDE_DIR = $$HOME/llvm-project/clang/include
LIBCLANG_LIB_DIR = $$HOME/llvm-project/build/lib

CC = g++
CC_OPTION = -std=c++14 -g -Wall -I $(INCLUDE_DIR) -I $(LIBCLANG_INCLUDE_DIR)
LD_OPTION = -L $(LIBCLANG_LIB_DIR) -lclang -Wl,-rpath=$(LIBCLANG_LIB_DIR)

KEYEDGE_INFORMATION_OBJECTS = type_information.o type_indicator.o element_information.o \
function_information.o primitive_type_information.o struct_information.o \
array_information.o pointer_information.o global.o
KEYEDGE_EMITTER_OBJECTS = basic.o array_serialization.o struct_serialization.o \
pointer_serialization.o serialization.o emit_fbs.o emit_common.o emit_eapp.o emit_host.o
KEYEDGE_OBJECTS = parser.o main.o \
$(addprefix information/, $(KEYEDGE_INFORMATION_OBJECTS)) \
$(addprefix emitter/, $(KEYEDGE_EMITTER_OBJECTS))
KEYEDGE_LIST = $(addprefix $(BIN_DIR)/, $(KEYEDGE_OBJECTS))

RISCV_CC = riscv64-unknown-linux-gnu-gcc
FLATCC_CC_OPTION = -I$(FLATCC_DIR)/include -Wall -Werror
FLATCC_OBJECTS = builder.o emitter.o refmap.o verifier.o
FLATCC_LIST = $(addprefix $(LIB_DIR)/, $(FLATCC_OBJECTS))

all : directory keyedge flatcc

directory :
	mkdir -p $(BIN_DIR)/information
	mkdir -p $(BIN_DIR)/emitter
	mkdir -p $(BIN_DIR)
	mkdir -p $(LIB_DIR)
	
keyedge : $(BIN_DIR)/keyedge

-include $(KEYEDGE_LIST:.o=.d)

$(BIN_DIR)/keyedge : $(KEYEDGE_LIST)
	$(CC) $(KEYEDGE_LIST) -o $@ $(LD_OPTION)

$(BIN_DIR)/information/%.o : $(SRC_DIR)/information/%.cpp $(INCLUDE_DIR)/index.h
	$(CC) $(CC_OPTION) -c $< -o $@
	$(CC) $(CC_OPTION) -MM $< > $(@:.o=.d)
	
$(BIN_DIR)/emitter/%.o : $(SRC_DIR)/emitter/%.cpp $(INCLUDE_DIR)/index.h
	$(CC) $(CC_OPTION) -c $< -o $@
	$(CC) $(CC_OPTION) -MM $< > $(@:.o=.d)
	
$(BIN_DIR)/%.o : $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/index.h
	$(CC) $(CC_OPTION) -c $< -o $@
	$(CC) $(CC_OPTION) -MM $< > $(@:.o=.d)
	
flatcc : $(LIB_DIR)/flatccrt.a

$(LIB_DIR)/flatccrt.a : $(FLATCC_LIST)
	ar ru $(LIB_DIR)/flatccrt.a $(FLATCC_LIST)
	ranlib $(LIB_DIR)/flatccrt.a

$(LIB_DIR)/%.o : $(FLATCC_DIR)/src/runtime/%.c
	$(RISCV_CC) $(FLATCC_CC_OPTION) -c $< -o $@

clean :
	rm -rf bin
	rm -rf lib
