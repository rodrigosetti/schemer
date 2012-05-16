# Schemer

This is a minimalist scheme interpreter implemented in C++. The main purpose of
this project is educational.

It implements a subset of scheme specification, yet, is fully computational
capable (_i. e._ it's Turing complete)

## Features

### Language

* `(define <name> <expression>)`
* `(lambda (<formal parameter>+> <expression>)`
* `(cond (<condition> <expression>)+ )`
* `(if <condition> <consequent> <otherwise>)`
* `(quote <expression>)`
* `(include <file path>)`

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
* `(putchar <number>)`
* `(getchar)`

### Techincal

* Uses great C++ STL library
* Mark and Sweep garbage collector
* interpreter mode (given command line file name)
* eval-print-loop mode

## How to use

After compiling the project with `make`, the `schemer` executable can be
used whether as an eval-print-loop or file interpreter (if given filename in
command line):

    ./schemer [<.scm file>]

In eval-print-loop state (_i. e._ without parameter), the user can input line
expressions and see it's evaluation. To exit the evaluator just hit `Ctrl-D`.

Errors normaly should not crash the interpreter. Rather, it should print the
error message and continue to the next expression input line.

### Tips

If a symbol needs to have a space, one can use double quotes (") to delimit
such token (this can be useful for file path, that must consist of a single
symbol), _e. g._:

    (include "examples/test 1.scm")

## TODO

* Garbage Collector in interpreter mode
* Quotation syntax sugar (_i. e._ `'(quoted expression)`)
* Tail call optimization

## Future features:

* Scheme macros
* Compiling option

