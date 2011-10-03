#include "expression.h"
#include "application.h"
#include "atom.h"
#include "begin.h"
#include "builtin.h"
#include "cond.h"
#include "include.h"
#include "define.h"
#include "if.h"
#include "lambda.h"
#include "procedure.h"
#include "quote.h"

#include <sstream>

using namespace std;

Token *next_token(list<Token*> &tokens) {

    Token *token = tokens.front();
    tokens.pop_front();
    return token;
}

void expectOpen(list<Token*> &tokens) throw (SchemerException*) {
    Token *token = next_token(tokens);
    if (token->type != TOK_OPEN) {
        throw new SchemerException("Expected (", token->line, token->column);
    }
}

void expectClose(list<Token*> &tokens) throw (SchemerException*) {
    Token *token = next_token(tokens);
    if (token->type != TOK_CLOSE) {
        throw new SchemerException("Expected )", token->line, token->column);
    }
}

Expression* Expression::parse(list<Token*> &tokens) throw (SchemerException*) {

    if ( tokens.empty() )
        throw new SchemerException("Unexpected end of input.");

    Token *token;

    // consume the next token
    token = next_token(tokens);

    if (token->type == TOK_OPEN) {

        // consume the next token
        token = next_token(tokens);

        if (token->type == TOK_RESERVED) {
            switch (((ReservedWordToken*)token)->reservedWord) {
                case RES_LAMBDA :
                    return LambdaExpression::parse(tokens);
                case RES_DEFINE :
                    return DefineExpression::parse(tokens);
                case RES_IF :
                    return IfExpression::parse(tokens);
                case RES_COND :
                    return CondExpression::parse(tokens);
                case RES_QUOTE :
                    return QuoteExpression::parse(tokens);
                case RES_BEGIN :
                    return BeginExpression::parse(tokens);
                case RES_INCLUDE :
                    return IncludeExpression::parse(tokens);
            }
        }
        else {
            // put back next token, because it will be used by application
            tokens.push_front(token);
            return ApplicationExpression::parse(tokens);
        }
    }
    else if (token->type == TOK_CLOSE) {
        throw new SchemerException("Unexpected closing parenthesis",
                token->line, token->column);
    }
    else {
        return new Atom(token);
    }
}

ostream & operator << (ostream &output, const Expression *expression) {
    switch (expression->type) {
        case EXP_ATOM:
            output << (Atom*)expression;
            break;
        case EXP_DEFINE:
            output << (DefineExpression*)expression;
            break;
        case EXP_LAMBDA:
            output << (LambdaExpression*)expression;
            break;
        case EXP_BEGIN:
            output << (BeginExpression*)expression;
            break;
        case EXP_IF:
            output << (IfExpression*)expression;
            break;
        case EXP_COND:
            output << (CondExpression*)expression;
            break;
        case EXP_QUOTE:
            output << (QuoteExpression*)expression;
            break;
        case EXP_APPLICATION:
            output << (ApplicationExpression*)expression;
            break;
        case EXP_INCLUDE:
            output << (IncludeExpression*)expression;
            break;
        case EXP_PROCEDURE:
            output << (Procedure*)expression;
            break;
        case EXP_BUILTIN:
            output << (BuiltInProcedure*)expression;
            break;
    }

    return output;
}

