SRC_DIR = src

CC = g++
CC_OPTION = -std=c++11 -g -Wall -I$(SRC_DIR)

keyedge : tokenizer.o parser.o emitter.o main.o
	$(CC) $(CC_OPTION) tokenizer.o parser.o emitter.o main.o -o keyedge

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
