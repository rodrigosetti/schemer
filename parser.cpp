#include "parser.h"
#include <sstream>

using namespace std;

Expression::Expression(const ExpressionType t, Token *token) {
    this->type = t;
    this->token = token;
}

Expression Expression::parse(std::list<Token*> &tokens) throw (SchemerException) {

    if ( tokens.empty() )
        throw SchemerException("Unexpected end of input.");

    Expression expr;
    Token *token = tokens.front();
    list<Expression> inner;
    istringstream test(token->symbolValue);
    long int intVal;
    long double floatVal;

    tokens.pop_front(); // consume token

    switch (token->type) {
        case TOK_OPEN:
            expr = Expression(EXP_COMPOSITE, token);

            while (true) {

                if (tokens.empty())
                    throw SchemerException("Unmatched parenthesis", token->line, token->column);
                if (tokens.front()->type == TOK_CLOSE) {
                    tokens.pop_front(); // consume token
                    break;
                }

                inner.push_back( parse( tokens ));
            }
            expr.innerExpressions = inner;
            break;
        case TOK_SYMBOL:
            if (test >> intVal)
            {
                expr = Expression(EXP_INT, token);
                expr.intValue = intVal;
            }
            else if (test >> floatVal) {
                expr = Expression(EXP_FLOAT, token);
                expr.floatValue = floatVal;
            }
            else {
                expr = Expression(EXP_SYMBOL, token);
                expr.symbolValue = token->symbolValue;
            }
            break;
        default:
            expr = Expression(EXP_SPECIAL, token);
            break;
    }

    return expr;
}

void Expression::print(ostream &output) {

    list<Expression>::iterator i;
    list<string>::iterator j;
    bool sep;

    switch (type) {
        case EXP_SYMBOL:
            output << symbolValue;
            break;
        case EXP_SPECIAL:
            token->print(output);
            break;
        case EXP_FLOAT:
            output << floatValue;
            break;
        case EXP_INT:
            output << intValue;
            break;
        case EXP_COMPOSITE:

            output << '(';
            sep = false;
            for (i = innerExpressions.begin();
                 i != innerExpressions.end();
                 i++) {
                if (sep) output << ' ';
                i->print(output);
                sep = true;
            }
            output << ')';
            break;
        case EXP_LAMBDA:
            output << "(LAMBDA (";
            sep = false;
            for (j = lambdaFormalParameters.begin();
                 j != lambdaFormalParameters.end();
                 j++) {
                if (sep) output << ' ';
                output << *j;
                sep = true;
            }
            output << ") ";
            lambdaExpression->print(output);
            output << ")";
            break;
        case EXP_BUILTIN:
            output << "<BUILTIN>";
            break;
    }
    output << "<" << token->line << "," << token->column << ">";
}

