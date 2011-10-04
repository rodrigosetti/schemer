;; This file provides some essencial scheme stuff to enrich
;; interpreter capabilities

(begin

    (define empty nil)

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

    (define not
        (lambda (x)
            (if x
                #f
                #t)))

    (define or
        (lambda (x y)
                (cond 
                    (x #t)
                    (y #t)
                    (#t #f))))

    (define and
        (lambda (x y)
                (if x
                    (if y
                        #t
                        #f)
                    #f)))

    (define >=
        (lambda (x y)
                (or (= x y) (> x y))))

    (define <=
        (lambda (x y)
                (or (= x y) (< x y))))

    (define cons
        (lambda (x y)
            (lambda (z)
                (if (= 0 z)
                    x
                    y))))

    (define car
        (lambda (z) (z 0)))

    (define cdr
        (lambda (z) (z 1)))

    (define list
        (lambda (x)
            (cons x empty)))

    (define isempty?
        (lambda (l)
            (= l empty)))

    (define range
        (lambda (min max)
                (if (= min max)
                    empty
                    (if (< min max)
                        (cons min (range (+ min 1) max))
                        (cons min (range (- min 1) max))))))

    (define length
        (lambda (l)
            (if (isempty? l)
                0
                (+ 1 (length (cdr l))))))

    (define filter
        (lambda (f l)
            (if (isempty? l)
                empty
                (if (f (car l))
                    (cons (car l) (filter f (cdr l)))
                    (filter f (cdr l))))))

    (define map
        (lambda (f l)
            (if (isempty? l)
                empty
                (cons (f (car l)) (map f (cdr l))))))

    (define fold
        (lambda (f l s)
            (if (isempty? l)
                s
                (fold f (cdr l)
                    ((f s) (car l))))))

    (define sum
        (lambda (l)
            (fold + l 0)))

    (define product
        (lambda (l)
            (fold * l 1)))

    (define avg
        (lambda (l)
            (/ (sum l) (length l))))

    (define min
        (lambda (x y)
            (if (< y x)
                y
                x)))

    (define max
        (lambda (x y)
            (if (< y x)
                x
                y)))

    (define list-min
        (lambda (l)
            (fold min l (car l))))

    (define list-max
        (lambda (l)
            (fold max l (car l))))

    (define take
        (lambda (n l)
                (if (or (isempty? l) (= 0 n))
                    empty
                    (cons (car l) (take (- n 1) (cdr l))))))

    (define take-while
        (lambda (f l)
                (if (isempty? l)
                    empty
                    (if (f (car l))
                        (cons (car l) (take-while f (cdr l)))
                        empty))))

    (define any
        (lambda (l)
            (fold or l #f)))

    (define all
        (lambda (l)
            (fold and l #t)))

    (define join
        (lambda (x y)
            (if (isempty? x)
                y
                (cons (car x) (join (cdr x) y)))))

    ;; Quicksort implementation using list's first element
    ;; as pivot
    (define sort
        (lambda (l)
            (if (isempty? l)
                empty
                (begin
                    (define pivot (car l))
                    (join
                        (sort (filter (lambda (e) (<= e pivot)) (cdr l)))
                        (cons pivot
                            (sort (filter (lambda (e) (> e pivot)) (cdr l)))))))))

)

