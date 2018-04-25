(defparameter +port+ 8080)
(defparameter +localhost+ "127.0.0.1")

(defun read-socket-line (stream)
  (if (peek-char nil stream nil nil)
      (coerce (loop while (listen stream)
                 collect (read-char stream))
              'string)))
