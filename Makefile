all: run

run: main
	./main

main: main.c
	gcc -o main main.c -lncurses

debug: main.c
	gcc -o main main.c -lncurses -g

clean:
	@rm main
