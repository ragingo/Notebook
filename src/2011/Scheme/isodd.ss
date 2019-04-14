
;-----------------------------------------------------
(define (isOdd n)
  (eqv? (remainder n 2) 1)
)
;-----------------------------------------------------
(define (where seq pred)
  (if (null? seq)
      '()
      (if (pred (car seq))
          (cons (car seq)
                (where (cdr seq) pred))
          (where (cdr seq) pred)
      )
  )
)
(for-each display
          (where (list 1 2 3 4 5) odd?)
)






(define (for from to step func)
  (if (>= from to)
      from
      (for (+ from step) to step func)
  )
  (func from)
)
(for 0 9 3 display)


(define (for-iter from to step func curval)
  
)


(newline)
(for-each display (append (list 4 9) (list 4 9)))
(newline)
(for-each display (list (cons 4 9)))


