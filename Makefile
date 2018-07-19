GCC = gcc
FLAGS = -I . -Wall

CC = ${GCC} ${FLAGS}

all: test c

c: c-server #c-client
c-%: c-%.o
	${CC} -o $* $*.o
c-%.o: %.c
	${CC} -c $*.c

test: test-server test-client
test-server: test-server.o
test-client: test-client.o
test-%.o: test-%.c
	${CC} -c test-$*.c
test-%: test-%.o
	${CC} -o test-$* test-$*.o

clean: clean-test clean-c
clean-o:
	rm -rf *.o
clean-test: clean-o
	rm -rf test-server test-client
	rm -rf test-server.exe test-client.exe
clean-c: clean-o
	rm -rf c-server c-client
	rm -rf c-server.exe c-client.exe
