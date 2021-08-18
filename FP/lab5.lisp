(defclass cart ()                
	((x :initarg :x :reader cart-x)
	 (y :initarg :y :reader cart-y)))

(defmethod print-object ((c cart) stream)
	(format stream "[CART x ~d y ~d]"
		(cart-x c) (cart-y c)))

(defclass polar ()
	((radius :initarg :radius :accessor radius)
		(angle  :initarg :angle  :accessor angle)))

(defmethod print-object ((p polar) stream)
	(format stream "[POLAR radius ~d angle ~d]"
		(radius p) (angle p)))

(defmethod radius ((c cart))
  (sqrt (+ (* (cart-x c) (cart-x c))
           (* (cart-y c) (cart-y c)))))

(defmethod angle ((c cart))
  (atan (cart-y c) (cart-x c)))

(defmethod cart-x ((p polar))
	(* (cos (angle p)) (radius p)))

(defmethod cart-y ((p polar))
	(* (sin (angle p)) (radius p)))

(defgeneric to-cart (arg)
	(:method ((c cart)) c)
	(:method ((p polar))
		(make-instance 'cart :x (cart-x p) :y (cart-y p))))

(defclass line ()
	((start :initarg :start :accessor line-start)
	 (end   :initarg :end   :accessor line-end)))

(defmethod print-object ((lin line) stream)
	(format stream "[ОТРЕЗОК ~s ~s]"
		(line-start lin) (line-end lin)))

(defclass triangle ()
	((vertex1 :initarg :1 :reader vertex1)
	 (vertex2 :initarg :2 :reader vertex2)
	 (vertex3 :initarg :3 :reader vertex3)))

(defmethod print-object ((tri triangle) stream)
	(format stream "[ТРЕУГ ~s ~s ~s]"
		(vertex1 tri) (vertex2 tri) (vertex3 tri)))

(defgeneric containing-rect (shape))
(defmethod containing-rect ((tri triangle))
(let* ((minX 0)
      (maxX 0)
      (minY 0)
      (maxY 0)
      (aX (cart-x (to-cart (vertex1 tri))))
      (bX (cart-x (to-cart (vertex2 tri))))
      (cX (cart-x (to-cart (vertex3 tri))))
      (aY (cart-y (to-cart (vertex1 tri))))
      (bY (cart-y (to-cart (vertex2 tri))))
      (cY (cart-y (to-cart (vertex3 tri))))
      )

      (setq minX (min aX bX cX))
      (setq maxX (max aX bX cX))
      (setq minY (min aY bY cY))
      (setq maxY (max aY bY cY))
      (setq minX (- minX 1))
      (setq minY (- minY 1))
      (setq maxX (+ maxX 1))
      (setq maxY (+ maxY 1))
      (list (make-instance 'cart :x minX :y minY)
            (make-instance 'cart :x minX :y maxY) 
            (make-instance 'cart :x maxX :y minY)
            (make-instance 'cart :x maxX :y maxY))))

(defvar triangleCartesian (make-instance 'triangle
           :1 (make-instance 'cart :x 1 :y 5)
           :2 (make-instance 'cart :x 10 :y 10)
           :3 (make-instance 'cart :x 4 :y 110)))
(defvar trianglePolar (make-instance 'triangle
           :1 (make-instance 'polar :radius (radius (vertex1 triangleCartesian)) :angle (angle (vertex1 triangleCartesian)))
           :2 (make-instance 'polar :radius (radius (vertex2 triangleCartesian)) :angle (angle (vertex2 triangleCartesian)))
           :3 (make-instance 'polar :radius (radius (vertex3 triangleCartesian)) :angle (angle (vertex3 triangleCartesian)))))
(print (containing-rect triangleCartesian))
(print "------------------------------------")
(print (containing-rect trianglePolar))