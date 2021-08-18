(defun funct (n &optional (g 1) (c 1) (p 1))
  (if (< n 3) g (funct (- n 1) (+ (+ g c) p) g c )))
