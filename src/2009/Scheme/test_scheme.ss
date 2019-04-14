; http://www.sampou.org/scheme/t-y-scheme/t-y-scheme-Z-H-1.html
; http://www.shido.info/
; http://www.kt.rim.or.jp/~kbk/guile/guile_toc.html

;---------------------------------------------------------------
; factorial
;---------------------------------------------------------------
(define (fact n)
  (fact-iter 1 1 n)
)
(define (fact-iter value count max)
  (cond ((< max 0) 0)
        ((< count max) (fact-iter (* value (+ count 1)) (+ count 1) max))
        (else value)
  ) ; cond
) ; define
(define (fact2 n)
  (fact-iter2 n n)
)
; version 2
(define (fact-iter2 x y)
  (if (= x 1)
      y
      (let ((_x (- x 1)))
        (fact-iter2 _x (* _x y))
      ) ; let
  ) ;if
)
;---------------------------------------------------------------
; absolute
;---------------------------------------------------------------
(define (abs2 x)
  (if (< x 0) (- x)
      x
  ) ;if
) ; define

;---------------------------------------------------------------
; power
;---------------------------------------------------------------
(define (pow2 x y)
  (let ((next-y (- y 1)))
    (if (< next-y 1)
        x
        (* x (pow2 x next-y))
    ) ; if
  ) ; let
) ; define

;---------------------------------------------------------------
; fibonacci (Fib(n) -> n=0:0, n=1:1, n>1:Fib(n-1)+Fib(n-2))
;---------------------------------------------------------------
(define (fib x)
  (cond ((< x 2) x)
        (else (+ (fib (- x 1)) (fib (- x 2))))
  ) ; cond
) ; define

;---------------------------------------------------------------
; tribonacci (Fib(n) -> n=0:0, n=1:0, n=2:1, n>1:Fib(n-1)+Fib(n-2))
;---------------------------------------------------------------
(define (trib x)
  (cond ((< x 2) 0)
        ((= x 2) 1)
        (else (+ (trib (- x 1)) (trib (- x 2)) (trib (- x 3))))
  ) ; cond
) ; define

;---------------------------------------------------------------
; pi, pi2
;---------------------------------------------------------------
(define (pi)
  3.1415926535897932384626433832795028841971693993751058209749445923078164062862089
)
(define (pi2)
  (* (atan 1) 4)
)

;---------------------------------------------------------------
; degree radian converter (360 deg = 2 pi)
;---------------------------------------------------------------
(define (degree-to-radian x)
  (/ (* x (pi)) 180)
)
(define (radian-to-degree x)
  (* x (/ 180 (pi)))
)

;---------------------------------------------------------------
; permutation
;---------------------------------------------------------------
(define (permutation n r)
  (let ((_n (fact n))(_nr (fact (- n r))))
    (/ _n _nr)
  ) ; let
) ; define

;---------------------------------------------------------------
; combination
;---------------------------------------------------------------
(define (combination n r)
  (let ((_p (permutation n r))(_r (fact r)))
    (/ _p _r)
  ) ; let
) ; define

;---------------------------------------------------------------
; heron
;---------------------------------------------------------------
(define (heron a b c)
  (let ((s (* 0.5 (+ a b c))))
    (sqrt (* s (- s a) (- s b) (- s c)))
  ) ; let
); define

;---------------------------------------------------------------
; area
;---------------------------------------------------------------
(define (rectangle a b)
  (* a b)
)
(define (square a)
  (pow2 a 2)
)

;---------------------------------------------------------------
; volume
;---------------------------------------------------------------
(define (trigonalpyramid r h)
  (/ (* r r (pi) h) 3)
)

;---------------------------------------------------------------
; test code
;    #t:true, #f:false
;    eq?:address
;    eqv?:data type & value
;    equal?:list or string
;---------------------------------------------------------------
(define (test-func1)
  (begin
    (display "you give love a bad name")(newline)
    (display "something for the pain")(newline)
  )
)
; ex.) > (test-func2 #t), > (test-func2 #f)
(define (test-func2 pred)
  (when pred ; true only
    (display "you give love a bad name")(newline)
    (display "something for the pain")(newline)
  )
)
; ex.) > (test-func3 #t), > (test-func3 #f)
(define (test-func3 pred)
  (unless pred ; true only
    (display "you give love a bad name")(newline)
    (display "something for the pain")(newline)
  )
)
(define global-variable1 "global-variable1")
(define (test-func4)
  (eq? global-variable1 "global-variable1") ; another address
)

;---------------------------------------------------------------
; volume
;---------------------------------------------------------------
(define (test-sort)
  (display (sort  '( 12 3 5 8 9 4 10 1 2 3) <))
)





