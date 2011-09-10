#include "parser.h"

bool is_digit(char c) {
    return ((c >= '0') && (c <= '9'));
}

void destroy_expression(t_expression **expr) {

    t_expression *p;
    t_expression *next;

    switch ((*expr)->type) {

        case EXP_COMPOSITE:
            p = (*expr)->data.composite;
            while (p != NULL) {
                next = p->next;
                destroy_expression(&p);
                p = next;
            }
            break;
        case EXP_LAMBDA:
            destroy_expression(&((*expr)->data.lambda.lambda_expr));
            break;
    }

    free(*expr);
    *expr = NULL;
}

t_expression *parse(t_token **token) {

    char *c;
    bool is_number;
    t_expression *next_expr;
    t_expression *tail;
    t_expression *expr = malloc(sizeof(t_expression));
    expr->token = *token;
    expr->next = NULL;

    if ((*token)->type == TOK_HEAD)
        *token = (*token)->next;

    switch ((*token)->type) {
        case TOK_OPEN:
            *token = (*token)->next; // consume token
            expr->type = EXP_COMPOSITE;
            expr->data.composite = NULL;

            tail = NULL;
            while (true) {
                if (*token == NULL) {
                    free(expr);
                    exit(1);
                }
                if ((*token)->type == TOK_CLOSE) {
                    *token = (*token)->next; // consume token
                    break;
                }
                else {
                    next_expr = parse(token);
                    if (tail != NULL) tail->next = next_expr;
                    if (expr->data.composite == NULL) expr->data.composite = next_expr;
                    tail = next_expr;
                }
            }
            break;
        case TOK_SYMBOL:
            /* ATOM case */

            /* checks if its a number */
            is_number = true;
            for (c = (*token)->value; *c != '\0'; c++)
                if (!is_digit(*c)) {
                    is_number = false;
                    break;
                }

            if (is_number) {
                /* is a number, parse int */
                expr->type = EXP_NATURAL;
                expr->data.value = atoi((*token)->value);
            }
            else {
                /* regular symbol or reserved word */
                expr->type = EXP_SYMBOL;
            }
            *token = (*token)->next; // consume token
            break;
        default:
            *token = (*token)->next; // consume token
            expr->type = EXP_SYMBOL;
    }

    return expr;
}
