CC=cc
CFLAGS=-std=c99 -O2 -Wall -Wextra -g3
INC=-Iinclude
LIB=$(shell pkg-config --libs glfw3) $(shell pkg-config --libs glew) -lm
BIN=minesweeper

SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,%.o,$(SRC))

default: $(BIN)

$(BIN): $(OBJ)
	@echo Linking executable...
	@$(CC) $(CFLAGS) $^ -o $@ $(LIB)
	@echo Successfully generated "'"$(BIN)"'"!
	@rm -f $(OBJ)

%.o: src/%.c
	@echo Compiling source file "'"$^"'"...
	@$(CC) $(CFLAGS) -c $^ $(INC)

clean:
	@echo Cleaning up workspace...
	@rm -rf $(BIN) $(OBJ)
