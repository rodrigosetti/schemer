(begin

    (define or
        (lambda (x y)
                (cond
                    (x #t)
                    (y #t)
                    (#t #f))))

    (display
        (or
            (begin
                (display 1001)
                #t)
            (begin
                (display 1002)
                #t)))
)

