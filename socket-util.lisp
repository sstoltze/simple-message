
(defun read-socket-line (stream)
  (coerce (loop while (listen stream)
             collect (read-char stream))
          'string))
