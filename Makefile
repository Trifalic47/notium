CC=gcc
SRC=./src/notium.c ./src/utils.c
OUTPUT=./build/notium
CFLAGS=-g -O0 -Wall -Wextra -fsanitize=address -lncurses

all:
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC)

run:
	$(OUTPUT)

clear:
	rm -r $(OUTPUT)
