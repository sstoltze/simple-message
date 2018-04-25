;; https://analogreader.wordpress.com/2015/07/13/socket-programming-in-common-lisp-how-to-use-usockets-with-macros/

(ql:quickload :usocket)

(load "socket-util.lisp")

(defun server ()
  (usocket:with-socket-listener (server +localhost+ +port+
                                        :reuse-address t)
    (usocket:with-connected-socket (connection (usocket:socket-accept server))
      (let ((stream (usocket:socket-stream connection)))
        (format t "Reading message:~%")
        (format t "~A~%" (read-socket-line stream))
                                        ; (read-socket-line stream))
        (format t "Sending message.~%")
        (format stream "Hello from lisp server.")
        (force-output stream)))))




