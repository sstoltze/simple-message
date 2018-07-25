.DEFAULT_GOAL = all
.PHONY: all clean clean-% test c c-%

GCC = gcc
FLAGS = -I . -Wall
CC = ${GCC} ${FLAGS}

all: test c

c: c-server #c-client
c-%: c-%.o
	${CC} -o c-$* c-$*.o
c-%.o: %.c
	${CC} -o c-$*.o -c $*.c

test: test-server test-client
test-%.o: test-%.c
	${CC} -c test-$*.c
test-%: test-%.o
	${CC} -o test-$* test-$*.o

clean: clean-test clean-c
clean-test: clean-o
	rm -rf test-server test-client
	rm -rf test-server.exe test-client.exe
clean-c: clean-o
	rm -rf c-server c-client
	rm -rf c-server.exe c-client.exe
clean-o:
	rm -rf *.o
