#ifndef __TOKENIZER__H_
#define __TOKENIZER__H_

#include <stdio.h>
#include <stdlib.h>

#define MAX_SYMBOL_LENGHT 255

typedef enum { TOK_BEGIN, TOK_SYMBOL, TOK_OPEN, TOK_CLOSE } token_type;

typedef struct token {
    unsigned int line;
    unsigned int column;
    token_type type;
    char *value;
    struct token *next;
}
t_token;

t_token *tokenize(FILE *stream);
void destroy_tokens( t_token **tokens);

#endif
// __TOKENIZER__H_
