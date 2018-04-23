+;; https://gist.github.com/shortsightedsid/a760e0d83a9557aaffcc
;; https://bitbucket.org/elliottslaughter/blackthorn-engine-3d/src/94bc17efa497af519c1500797c89e90a477eded0/src/examples/usocket/usocket.lisp?at=default&fileviewer=file-view-default
(ql:quickload :usocket)

(defun socket-client ()
  (usocket:with-client-socket (client stream "127.0.0.1" 8080)
                              (format stream "Test")
                              (force-output stream)
                              (format t (read-line stream nil))))


