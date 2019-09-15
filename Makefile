SRC_DIR = src

CLANG_BIN_DIR = $$HOME/llvm-project/build/bin
LIBCLANG_INCLUDE_DIR = $$HOME/llvm-project/clang/include
LIBCLANG_LIB = $$HOME/llvm-project/build/lib

# CC = $(CLANG_BIN_DIR)/clang++
CC = g++
CC_OPTION = -std=c++14 -g -Wall -I $(LIBCLANG_INCLUDE_DIR)
LD_OPTION = -L $(LIBCLANG_LIB) -lclang -Wl,-rpath=$(LIBCLANG_LIB)

keyedge : tokenizer.o parser.o emitter.o main.o
	$(CC) tokenizer.o parser.o emitter.o main.o -o keyedge $(LD_OPTION)

main.o : $(SRC_DIR)/tokenizer.h $(SRC_DIR)/parser.h $(SRC_DIR)/emitter.h $(SRC_DIR)/main.cpp
	$(CC) $(CC_OPTION) -c $(SRC_DIR)/main.cpp -o main.o

tokenizer.o : $(SRC_DIR)/tokenizer.h $(SRC_DIR)/tokenizer.cpp
	$(CC) $(CC_OPTION) -c $(SRC_DIR)/tokenizer.cpp -o tokenizer.o

parser.o : $(SRC_DIR)/tokenizer.h $(SRC_DIR)/parser.h $(SRC_DIR)/parser.cpp
	$(CC) $(CC_OPTION) -c $(SRC_DIR)/parser.cpp -o parser.o

emitter.o : $(SRC_DIR)/tokenizer.h $(SRC_DIR)/parser.h $(SRC_DIR)/emitter.h $(SRC_DIR)/emitter.cpp
	$(CC) $(CC_OPTION) -c $(SRC_DIR)/emitter.cpp -o emitter.o

clean :
	rm keyedge main.o tokenizer.o parser.o emitter.o
