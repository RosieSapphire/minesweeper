CC=g++
INC=-I./include/ -I/usr/include -I/usr/local/include/
LIB=-lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lm

CFLAGS=

SRC=main.cpp buffer.cpp minefield.cpp vec2.cpp
OBJ=main.o buffer.o minefield.o vec2.o

BIN=minesweeper

all: release

release: $(BIN)
release: CFLAGS += -O2

debug: $(BIN)
debug: CFLAGS += -Og -g3 -Wall -Werror

run: release
	clear
	./$(BIN)

gdb: debug
	clear
	gdb ./$(BIN) --tui

valgrind: debug
	clear
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --gen-suppressions=all ./$(BIN)

$(BIN): $(OBJ)
	$(CC) $^ -o $(BIN) $(LIB)
	rm -rf *.o
	@echo "COMPILED SUCCESSFULLY"

%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $^ $(INC)

clean:
	rm -rf $(BIN) *.o src/*.orig include/*.orig
	clear

format:
	astyle -A3 -s -f -xg -k3 -xj -v src/*.c
	astyle -A3 -s -f -xg -k3 -xj -v include/*.h
