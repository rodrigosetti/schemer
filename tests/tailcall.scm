(begin
    (include "lib/prelude.scm")

    (define factorial
        (lambda (x)
            (begin
                (define fact
                    (lambda (i acc)
                        (if (= i 0)
                            acc
                            (fact (- i 1) (* acc i)))))
                      (fact x 1))))

    (display (factorial 9000))
)

