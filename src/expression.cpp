#include "expression.h"
#include "atom.h"
#include "lambda.h"
#include "define.h"
#include "if.h"
#include "cond.h"
#include "quote.h"
#include "begin.h"
#include "include.h"
#include "application.h"
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

Expression::Expression(const ExpressionType type) : GarbageCollectable() {
    this->type = type;
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

istream & operator >> (istream &input, Expression **expression) throw (SchemerException*) {
    list<Token*> tokens;

    input >> tokens;
    if ( !tokens.empty() ) {
        *expression = Expression::parse(tokens);
    }

    return input;
}

ostream & operator << (ostream &output, const Expression *expression) {
    return expression->print(output);
}
