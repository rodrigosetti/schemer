#include "tokenizer.h"

#include <string.h>

typedef enum { COMMENT, WHITESPACE, SYMBOL_CHAR, OPEN_PAR, CLOSE_PAR} t_state;

t_token *build_token(unsigned int line, unsigned int column,
                     char type, char *value) {
    t_token *token = malloc(sizeof(t_token));
    token->line = line;
    token->column = column;
    token->type = type;
    token->value = value;
    token->next = NULL;
}

void destroy_tokens(t_token **head) {

    t_token *p = *head;
    t_token *next;

    while (p != NULL) {

        next = p->next;
        if (p->type == TOK_SYMBOL)
            free(p->value);
        free(p);

        p = next;
    }
}

t_token *tokenize(FILE *stream) {

    t_token *head = build_token(0, 0, TOK_BEGIN, NULL);
    char next;
    t_state last_state = WHITESPACE;
    t_state cur_state;
    unsigned int line = 1;
    unsigned int column = 1;
    unsigned int s = 0;
    t_token *current_token = head;
    char symbol_buffer[MAX_SYMBOL_LENGHT];
    char *symbol_str;

    while ( (next = fgetc(stream)) != EOF) {

        /* count lines and columns */
        if (next == '\n') {
            line ++;
            column = 1;
        }
        else column ++;

        /* if state is comment and not end of line,
         * continue to the next char */
        if (last_state == COMMENT && next != '\n')
            continue;

        switch (next) {
            case '(':
                cur_state = OPEN_PAR;
                current_token->next = 
                    build_token(line, column, TOK_OPEN, NULL);
                current_token = current_token->next;
                break;
            case ')':
                cur_state = CLOSE_PAR;
                current_token->next = 
                    build_token(line, column, TOK_CLOSE, NULL);
                current_token = current_token->next;
                break;
            case '\n':
            case ' ':
            case '\t':
                cur_state = WHITESPACE;
                break;
            case ';':
                cur_state = COMMENT;
                break;
            default:
                cur_state = SYMBOL_CHAR;
        }

        /* if reading a symbol is over, write it to a new token */
        if (last_state == SYMBOL_CHAR && cur_state != SYMBOL_CHAR) {

            symbol_buffer[s++] = '\0';
            symbol_str = malloc(sizeof(char) *s);
            strcpy(symbol_str, symbol_buffer);
            current_token->next = 
                build_token(line, column, TOK_SYMBOL, symbol_str);
            current_token = current_token->next;

            s = 0;
        }
        else if (cur_state == SYMBOL_CHAR) {
            symbol_buffer[s++] = next;

            if (s == MAX_SYMBOL_LENGHT) {
                fprintf(stderr, "Error in line %d, column %d: maximum symbol size violated\n", 
                        line, column);
                destroy_tokens(&head);
                exit(1);
            }
        }

        last_state = cur_state;
    }

    return head;
}
