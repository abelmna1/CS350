all: prog3ipc parser.o

prog3ipc: prog3ipc.c
	gcc -o prog3ipc prog3ipc.c -g -lrt

parser.o: parser.c
	gcc -c parser.c -g

clean:
	rm -f *~
	rm -f parser.o
	rm -f prog3ipc
