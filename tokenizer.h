#ifndef __TOKENIZER__H_
#define __TOKENIZER__H_

#include <stdio.h>
#include <stdlib.h>

#define MAX_SYMBOL_LENGHT 255

typedef enum { TOK_HEAD, TOK_SYMBOL, TOK_OPEN, TOK_CLOSE,
               TOK_LAMBDA, TOK_DEFINE, TOK_IF, TOK_BEGIN } token_type;

typedef char bool;
#define true 1
#define false 0

typedef struct token {
    unsigned int line;
    unsigned int column;
    token_type type;
    char *value;
    bool quoted;
    struct token *next;
}
t_token;

t_token *tokenize(FILE *stream);
void destroy_tokens( t_token **tokens);

#endif
// __TOKENIZER__H_
