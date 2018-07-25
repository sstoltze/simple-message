.DEFAULT_GOAL = all
.PHONY: all clean clean-% test

GCC = gcc
FLAGS = -I . -Wall
CC = ${GCC} ${FLAGS}

all: test

test: test-server test-client
test-server: test-server.o
test-client: test-client.o
test-%.o: test-%.c
	${CC} -c test-$*.c
test-%: test-%.o
	${CC} -o test-$* test-$*.o

clean: clean-test
clean-test: clean-o
	rm -rf test-server test-client
	rm -rf test-server.exe test-client.exe
clean-o:
	rm -rf *.o
