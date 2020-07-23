all: minibase.c
	gcc minibase.c -o minibase.o

run: minibase.o
	./minibase.o

clean: 
	rm *.o;