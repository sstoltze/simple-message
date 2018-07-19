GCC = gcc
FLAGS = -I . -Wall

CC = ${GCC} ${FLAGS}

all: test lisp

lisp: lisp-client lisp-server
lisp-client: client.lisp
	sbcl --load client.lisp --eval "(sb-ext:save-lisp-and-die \"lisp-client.exe\" :executable t :toplevel 'minimal-client)"
lisp-server: server.lisp
	sbcl --load server.lisp --eval "(sb-ext:save-lisp-and-die \"lisp-server.exe\" :executable t :toplevel 'server)"


test: test-server test-client
test-server: test-server.o
test-client: test-client.o
test-%.o: test-%.c
	${CC} -c test-$*.c
test-%: test-%.o
	${CC} -o test-$* test-$*.o

clean: clean-test clean-lisp
clean-test:
	rm -rf *.o
	rm -rf test-server test-client
clean-lisp:
	rm -rf lisp-client lisp-server
