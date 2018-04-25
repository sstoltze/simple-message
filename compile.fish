gcc server.c -o c-server
gcc client.c -o c-client
gcc minimal-client.c -o c-minimal

sbcl --load server.lisp --eval '(sb-ext:save-lisp-and-die "lisp-server.exe" :executable t :toplevel #\'server)'
sbcl --load client.lisp --eval '(sb-ext:save-lisp-and-die "lisp-client.exe" :executable t :toplevel #\'minimal-client)'
