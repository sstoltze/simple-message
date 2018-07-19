GCC = gcc
FLAGS = -I . -Wall

CC = ${GCC} ${FLAGS}

all: test ocaml

ocaml: ocaml-client # ocaml-server
ocaml-%: %.ml
	ocamlc unix.cma $*.ml -o ocaml-$*

test: test-server test-client
test-server: test-server.o
test-client: test-client.o
test-%.o: test-%.c
	${CC} -c test-$*.c
test-%: test-%.o
	${CC} -o test-$* test-$*.o

clean: clean-test clean-ocaml
clean-test:
	rm -rf *.o
	rm -rf test-server test-client
clean-ocaml:
	rm -rf ocaml-client.exe ocaml-server.exe
	rm -rf ocaml-client ocaml-server
