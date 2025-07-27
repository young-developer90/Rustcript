CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
SRC = src/main.c src/parser.c src/lexer.c
OBJ = $(SRC:.c=.o)
TARGET = rustcript

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
