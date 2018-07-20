.DEFAULT_GOAL = all
.PHONY: all clean clean-% test ocaml
GCC = gcc
FLAGS = -I . -Wall

CC = ${GCC} ${FLAGS}

all: test ocaml

ocaml: ocaml-client # ocaml-server
ocaml.exe:
	ocamlmktop unix.cma -o ocaml.exe
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
	rm -rf *.cmi *.cmo
	rm -rf ocaml-client.exe ocaml-server.exe
	rm -rf ocaml-client ocaml-server
