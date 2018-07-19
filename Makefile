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

clean-test:
	rm -rf *.o
	rm -rf test-server test-client
