cc := gcc

build: src/main.c
	$(cc) src/main.c -Wall -Wextra -Lbin -lraylib -Isrc -o bin/game_of_life.exe
