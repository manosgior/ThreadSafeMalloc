CFLAGS= -pthread

main: main.c libmalloc.so
	gcc $(CFLAGS) -o mallocTest main.c
	
libmalloc.so: malloc.h malloc.c
	gcc $(CFLAGS) -shared -o libmalloc.so -fPIC malloc.c

clean:
	rm -f mallocTest libmalloc.so *.o
