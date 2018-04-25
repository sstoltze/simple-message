;; https://analogreader.wordpress.com/2015/07/13/socket-programming-in-common-lisp-how-to-use-usockets-with-macros/

;; http://mihai.bazon.net/blog/howto-multi-threaded-tcp-server-in-common-lisp

(ql:quickload :usocket)

(load "socket-util.lisp")

(defun minimal-server ()
  (usocket:with-socket-listener (server +localhost+ +port+
                                        :reuse-address t)
    (usocket:with-connected-socket (connection (usocket:socket-accept server))
      (let ((stream (usocket:socket-stream connection)))
        (format t "Reading message:~%")
        (format t "~A~%" (read-socket-line stream))
        (format t "Sending message.~%")
        (format stream "Hello from lisp server.")
        (force-output stream)))))

(defun server (&key (host +localhost+) (port +port+))
  (usocket:with-socket-listener (server host
                                        port
                                        :reuse-address t
                                        :backlog 50)
    (let ((sockets (list server))
          (num-sock 0)
          (num-ready 0)
          (last-message ""))
      (loop
         (let* ((ready-sockets (usocket:wait-for-input sockets
                                                       :ready-only t))
                (len-sock  (length sockets))
                (len-ready (length ready-sockets)))
           (when (or (not (= num-sock len-sock))
                     (not (= num-ready len-ready)))               
             (format t "Outer loop. ~A connections, ~A ready.~%"
                     len-sock len-ready)
             (setf num-sock  len-sock
                   num-ready len-ready))
           (loop for sock in ready-sockets
              do
                (cond ((equal sock server)
                       ;; Server. Connect new client
                       ;; Set name? Data?
                       (let ((new-sock (usocket:socket-accept sock)))
                         (setf sockets (push new-sock sockets))
                         (format t "Added new client.~%")))
                      ((find sock sockets :test #'equal)
                       ;; Client
                       (let ((client-stream (usocket:socket-stream sock)))
                         (if (and (listen client-stream)
                                  (peek-char nil client-stream nil nil))
                             (let ((message (read-socket-line client-stream)))
                               ;; Send message
                               (format t "Sending message: ~A~%" message)
                               (setf last-message message)
                               (loop for client in sockets do
                                    (if (and (not (equal client server))
                                             ;(not (equal client sock))
                                             )
                                        (let ((s (usocket:socket-stream client)))
                                          (handler-case
                                              (progn
                                                (format s "~A" message)
                                                (force-output s))
                                            (sb-int:simple-stream-error ()
                                              (progn
                                                (format t "Deleting client due to error.~%")
                                                (setf sockets (delete sock
                                                                      sockets))
                                                (usocket:socket-close sock))))))))
                             ;; Disconnect
                             (progn
                               (format t "Deleting client due to listen.~%")
                               (setf sockets (delete sock sockets))
                               (usocket:socket-close sock)))))
                      (t nil))))))))


