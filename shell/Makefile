all: shell

CFLAGS=-g -O3

# $@ is the production's target
# $^ are the production's prerequisites

shell: mytoc.o shell.o
	cc -o $@ $^

mytoc.o: mytoc.c mytoc.h
	cc -c $(CFLAGS) mytoc.c

shell.o: shell.c mytoc.h shell.h
	cc -c $(CFLAGS) shell.c

clean: 
	rm -f *.o shell

demo: shell
	(echo first; echo "second line"; echo "third and last") | ./shell
