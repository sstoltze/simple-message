gcc test-server.c -o c-test-server
gcc test-client.c -o c-test-client

sbcl --load server.lisp --eval '(sb-ext:save-lisp-and-die "lisp-server.exe" :executable t :toplevel #\'server)'
sbcl --load client.lisp --eval '(sb-ext:save-lisp-and-die "lisp-client.exe" :executable t :toplevel #\'minimal-client)'
