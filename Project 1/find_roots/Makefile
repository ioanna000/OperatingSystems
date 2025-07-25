find_roots_lib: find_roots_lib.o libroots.a
	gcc -Wall -g find_roots_lib.o libroots.a -o find_roots_lib

find_roots_lib.o: find_roots_lib.c roots.h
	gcc -Wall -g -c find_roots_lib.c

libroots.a: roots.c roots.h
	gcc -Wall -g -c roots.c
	ar rcs libroots.a roots.o
clean:
	rm *.o *.a find_roots_lib
