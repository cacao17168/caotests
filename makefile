CC = gcc
TARGET = test_lib.c
FLAGS = -g -O0 -Wall -Wextra
NAME = -o test-framework

all: $(TARGET)
	$(CC) $(TARGET) $(FLAGS) $(NAME)
