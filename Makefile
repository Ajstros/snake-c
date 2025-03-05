all: run

run: main
	./main

main: main.c
	gcc -o main main.c snake.c queue.c -lncurses

debug: main.c
	gcc -o main main.c snake.c queue.c -lncurses -g
	gdb main --tui

clean:
	@rm main
