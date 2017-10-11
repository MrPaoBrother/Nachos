hello:main.o me.o
	gcc -o hello main.o me.o
main.o:main.c me.h
	gcc -c main.c
me.o:me.c me.h
	gcc -c me.c
clean:
	rm hello main.o me.o
