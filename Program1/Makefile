all: parser.o prog2tree 

parser.o: parser.c
	gcc -c parser.c -g

prog2tree: prog2tree.c
	gcc -o prog2tree prog2tree.c -g

clean:
	rm -f *~
	rm -f parser
	rm -f *.o
	rm -f prog2tree
	
