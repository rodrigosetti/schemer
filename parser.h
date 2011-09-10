#ifndef __PARSER_H__
#define __PARSER_H__

#include "tokenizer.h"

typedef enum {
    EXP_NATURAL,
    EXP_SYMBOL,
    EXP_COMPOSITE,
    EXP_LAMBDA,
    EXP_BUILTIN
}
expression_type;

typedef struct expression {

    expression_type type;
    t_token *token;
    struct expression *next;

    union {
        // NATURAL
        unsigned long int value;

        // LAMBDA
        struct {
            t_token *parameter;
            struct expression *lambda_expr;
            //t_environment *lambda_environment;
        } lambda;

        // BUILTIN
        struct expression* (*builtin) (struct expression*);

        // COMPOSITE
        struct expression *composite;
    } data;
}
t_expression;

t_expression *parse(t_token **tokens);

#endif

