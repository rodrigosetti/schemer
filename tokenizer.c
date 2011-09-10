#include "tokenizer.h"

#include <string.h>

typedef enum { COMMENT, WHITESPACE, SYMBOL_CHAR,
               OPEN_PAR, CLOSE_PAR, QUOTE_CHAR} t_state;

t_token *build_token(unsigned int line, unsigned int column,
                     char type, char *value, bool quoted) {

    t_token *token = malloc(sizeof(t_token));
    token->line = line;
    token->column = column;
    token->type = type;
    token->value = value;
    token->quoted = quoted;
    token->next = NULL;

    return token;
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

token_type check_reserved_symbol(char *symbol) {

    if (strcmp(symbol, "lambda") == 0)
        return TOK_LAMBDA;
    else if (strcmp(symbol, "define") == 0)
        return TOK_DEFINE;
    else if (strcmp(symbol, "if") == 0)
        return TOK_IF;
    else if (strcmp(symbol, "begin") == 0)
        return TOK_BEGIN;
    else
        return -1;
}

t_token *tokenize(FILE *stream) {

    t_token *head = build_token(0, 0, TOK_HEAD, NULL, false);
    char next;
    t_state last_state = WHITESPACE;
    t_state cur_state;
    unsigned int line = 1;
    unsigned int column = 0;
    unsigned int s = 0;
    token_type reserved_symbol;
    t_token *current_token = head;
    char symbol_buffer[MAX_SYMBOL_LENGHT];
    char *symbol_str;
    bool quoted = false;

    while ( (next = fgetc(stream)) != EOF) {

        /* count lines and columns */
        if (next == '\n') {
            line ++;
            column = 0;
        }
        else column ++;

        /* if state is comment and not end of line,
         * continue to the next char */
        if (last_state == COMMENT && next != '\n')
            continue;

        switch (next) {
            case '(':
                cur_state = OPEN_PAR;
                break;
            case ')':
                if (quoted && last_state != SYMBOL_CHAR) {
                    fprintf(stderr, "Error in line %d, column %d: cannot quote before ')'\n", 
                            line, column);
                    destroy_tokens(&head);
                    exit(1);
                }
                cur_state = CLOSE_PAR;
                break;
            case '\n':
            case ' ':
            case '\t':
                cur_state = WHITESPACE;
                break;
            case ';':
                cur_state = COMMENT;
                break;
            case '\'':
                if (last_state != SYMBOL_CHAR)
                    quoted = false;
                else {
                    fprintf(stderr, "Error in line %d, column %d: Illegal char \"'\" in token\n", 
                            line, column);
                    destroy_tokens(&head);
                    exit(1);
                }
                cur_state = QUOTE_CHAR;
                quoted = true;
                break;
            default:
                cur_state = SYMBOL_CHAR;
        }

        /* if reading a symbol is over, write it to a new token */
        if (last_state == SYMBOL_CHAR && cur_state != SYMBOL_CHAR) {

            symbol_buffer[s++] = '\0';

            /* checks if symbols is a reserved word (except when quoted) */
            if (!quoted)
                reserved_symbol = check_reserved_symbol(symbol_buffer);

            /* if's not a reserved symbol, create a symbol token */
            if (quoted || reserved_symbol == -1) {
                symbol_str = malloc(sizeof(char) *s);
                strcpy(symbol_str, symbol_buffer);
                current_token->next = 
                    build_token(line, column, TOK_SYMBOL, symbol_str, quoted);
                quoted = false;
            }
            else {
                /* else, create a reserved symbol token */
                current_token->next = 
                    build_token(line, column, reserved_symbol, NULL, false);
            }
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

        /* adds apropriate token for opening or closing parenthesis */
        if (cur_state == OPEN_PAR) {
            current_token->next = 
                build_token(line, column, TOK_OPEN, NULL, quoted);
            current_token = current_token->next;
            quoted = false;
        }
        else if (cur_state == CLOSE_PAR) {
            current_token->next = 
                build_token(line, column, TOK_CLOSE, NULL, false);
            current_token = current_token->next;
        }

        last_state = cur_state;
    }

    return head;
}
