(defun check (a z) ;проверка заданного условия
    (if (= z (- (/ (length a) 2) 1))
        T
        (let (
            (xi (car (last a (- (length a) z)))) 
            (x1 (car (last a (+ z 1)))) ;x[2n-i+1]
            (x2 (car (last a (+ (/ (length a) 2) (+ z 1))))) ;x[n-i+1]
            )
            (if (= xi (+ x1 (+ x2 x2)))                
                (check a (+ z 1))
                NIL))))

(defun start (a)
    (if (= (mod (length a) 2) 0)
    (check a 0)
        NIL))