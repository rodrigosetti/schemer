# Schemer

This is a minimalist scheme interpreter implemented in C++. The main purpose of
this project is educational.

It implements a subset of scheme specification, yet, is fully computational
capable (i.e. it's Turing complete)

## Features

### Language

* `(define <name> <expression>)`
* `(lambda (<formal parameter>+> <expression>)`
* `(cond (<condition> <expression>)+ )`
* `(if <condition> <consequent> <otherwise>)`
* `(quote <expression>)`

### Builtin types and values

* integer, float (`.` for decimal), and negative numbers
* boolean values as `#t` and `#f`
* `nil` value
* symbols by using quotation

### Builtin functions

* `(+ <number>+)`
* `(* <number>+)`
* `(/ <divident> <divisor>)`
* `(% <divident> <divisor>)`
* `(< <number> <number>)`
* `(> <number> <number>)`
* `(= <expression> <expression>)`
* `(display <expression>)`

### How to use

After compiling the project with `make`, the `schemer` executable it's an
eval-print-loop interpreter and can accept scheme source file as command line
parameters, the usage is very simple:

    ./schemer [<.scm file>]*

Each scheme source file will be evaluated before entering the eval-print-loop
state. In this state, the user can input line expressions and see it's
evaluation. To exit the evaluator just hit `Ctrl-D`.

Errors normaly should not crash the interpreter. Rather, it should print the
error message and continue to the next expression input line.

## TODO

* Garbage Collector
* Tail call optimization
* More input/output function

## Future features:

* Scheme macros
* Compiling option

