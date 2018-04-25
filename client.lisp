;; https://gist.github.com/shortsightedsid/a760e0d83a9557aaffcc
;; https://bitbucket.org/elliottslaughter/blackthorn-engine-3d/src/94bc17efa497af519c1500797c89e90a477eded0/src/examples/usocket/usocket.lisp?at=default&fileviewer=file-view-default

;; https://analogreader.wordpress.com/2015/07/13/socket-programming-in-common-lisp-how-to-use-usockets-with-macros/
(ql:quickload :usocket)

(load "socket-util.lisp")

(defun send-socket (s &rest args)
  (apply #'format s args)
  (force-output s))

(defun minimal-client ()
  (usocket:with-client-socket (client stream +localhost+ +port+)
    (format t "Sending message.~%")
    (format stream "Hello from lisp client.")
    (force-output stream)
    (format t "Message received:~%")
    (format t "~A~%" (read-socket-line stream))))

(defun client-listener ()
  (usocket:with-client-socket (client stream +localhost+ +port+)
    (loop
       (format t "~A~%" (read-socket-line stream)))))

(defun client-sender ()
    (usocket:with-client-socket (client stream +localhost+ +port+)
      (loop
         (send-socket stream "~A~%" (read-line)))))

(defun client ()
  (usocket:with-client-socket (client stream +localhost+ +port+)
    ;; Input
    (loop
       (format stream "Test")
       (format t "~A~%" (read-socket-line stream)))
    ;; Print
    ))

