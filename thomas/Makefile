CC := /usr/bin/g++
MAKE := /usr/bin/make

SRC_DIR := ./src
OBJ_DIR := ./obj
INC_DIR := ./include
BIN_DIR := ./bin

#ifndef $(NAME)
	NAME := main
#endif

INCLS := $(wildcard $(INC_DIR)/*.h)
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:.c=.o)
OUTPUT = $(BIN_DIR)/$(NAME)

CFLAGS := -Wall -g 

.PHONY: all
all:
	$(CC) $(CFLAGS) $(SRCS) -o $(OUTPUT) -I$(INC_DIR)

$(OBJ_DIR)/%.o: %.c $(INCLS)
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INCLS)

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(NAME)
	

