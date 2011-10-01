(begin

    (define average
        (lambda (x y)
                (/ (+ x y) 2)))

    (define neg
        (lambda (x)
            (* -1 x)))

    (define -
        (lambda (x y)
            (+ x (neg y))))

    (define abs
        (lambda (x)
            (if (< x 0)
                (neg x)
                x)))

    (define square
        (lambda (x)
            (* x x)))

    (define ^
        (lambda (r e)
                (if (= 0 e)
                    1
                    (* r (^ r (- e 1))))))

    (define sqrt
        (lambda (x)
            (begin
                (define guess
                    (lambda (x g)
                            (if (< (abs (- (square g) x)) 0.001)
                                g
                                (guess x (average g (/ x g))))))
                (guess x (/ x 2)))))

    (sqrt 2)
)

