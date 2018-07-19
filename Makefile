GCC = gcc
GPP = g++
FLAGS = -I . -Wall

CC = ${GCC} ${FLAGS}
CPP = ${GPP} ${FLAGS}

all: test cpp

cpp: cpp-server # cpp-client
cpp-%: cpp-%.o
	${CPP} -o cpp-$* cpp-$*.o
cpp-%.o: %.cpp
	${CPP} -c -o cpp-$*.o $*.cpp

test: test-server test-client
test-%.o: test-%.c
	${CC} -c test-$*.c
test-%: test-%.o
	${CC} -o test-$* test-$*.o

clean: clean-test clean-cpp
clean-cpp: clean-o
	rm -rf cpp-server cpp-client
	rm -rf cpp-server.exe cpp-client.exe
clean-test: clean-o
	rm -rf test-server test-client
	rm -rf test-server.exe test-client.exe
clean-o:
	rm -rf *.o
