;; https://analogreader.wordpress.com/2015/07/13/socket-programming-in-common-lisp-how-to-use-usockets-with-macros/

(ql:quickload :usocket)

;(load "socket-util.lisp")
(defun read-socket-line (stream)
  (do ((c (read-char-no-hang stream) (read-char-no-hang stream))
       (res (list)))
      ((null c) (coerce (reverse res) 'string))
    (push c res)))

;(defun read-socket-line (stream)
;  (coerce (loop while (listen stream)
;             collect (read-char stream))
;          'string)
;  (clear-input stream)
;  )

(defconstant +port+ 8080)

(defun socket-server ()
  (usocket:with-socket-listener (server "127.0.0.1" +port+)
    (usocket:with-connected-socket (connection (usocket:socket-accept server))
      (let ((stream (usocket:socket-stream connection)))
        (format t "Reading message:~%")
        (format t "~A~%" (read-socket-line stream))
                                        ; (read-socket-line stream))
        (format t "Sending message.~%")
        (format stream "Hello from lisp server.")
        (force-output stream)))))




