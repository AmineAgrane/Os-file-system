all : programme

programme.o : programme.c fonctions.c head.h
	gcc -c programme.c

programme : programme.o
		gcc programme.o -o programme

clean :
	rm -f programme*.o
