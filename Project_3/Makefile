user_app: user_app.o libmemory_syscalls.a
	gcc -Wall -g user_app.o libmemory_syscalls.a -o user_app

user_app.o: user_app.c memory_syscalls.h
	gcc -Wall -g -c user_app.c

libmemory_syscalls.a: memory_syscalls.c memory_syscalls.h
	gcc -Wall -g -c memory_syscalls.c
	ar rcs libmemory_syscalls.a memory_syscalls.o

clean:
	rm user_app.o user_app memory_syscalls.o libmemory_syscalls.a
