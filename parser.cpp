#include "parser.h"
#include <sstream>

using namespace std;

Expression::Expression(const long double floatValue, Token *token) {
    type = EXP_FLOAT;
    this->floatValue = floatValue;
    this->token = token;
}
Expression::Expression(const long int intValue, Token *token) {
    type = EXP_INT;
    this->intValue = intValue;
    this->token = token;
}

Expression::Expression(Token *token) {
    type = EXP_SYMBOL;
    this->token = token;
}

Expression::Expression(const std::list<Expression> &innerExpressions, Token *token) {
    type = EXP_COMPOSITE;
    this->innerExpressions = innerExpressions;
    this->token = token;
}

Expression::Expression(const BuiltInMethod builtInValue, Token *token) {
    type = EXP_BUILTIN;
    this->builtInValue = builtInValue;
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
            while (true) {

                if (tokens.empty())
                    throw SchemerException("Unmatched parenthesis", token->line, token->column);
                if (tokens.front()->type == TOK_CLOSE) {
                    tokens.pop_front(); // consume token
                    break;
                }

                inner.push_back( parse( tokens ));
            }
            expr = Expression(inner, token);
            break;
        case TOK_SYMBOL:
            if (test >> intVal)
            {
                expr = Expression(intVal, token);
            }
            else if (test >> floatVal) {
                expr = Expression(floatVal, token);
            }
            else {
                expr = Expression(token);
            }
            break;
        default:
            expr = Expression(token);
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
}

Environment *Expression::getGlobalEnvironment() {

    map<string,Expression*> globals;

    globals.insert( pair<string,Expression*>( "+", new Expression(BI_ADD)));
    globals.insert( pair<string,Expression*>( "-", new Expression(BI_SUB)));
    globals.insert( pair<string,Expression*>( "*", new Expression(BI_MUL)));
    globals.insert( pair<string,Expression*>( "/", new Expression(BI_DIV)));
    globals.insert( pair<string,Expression*>( "display", new Expression(BI_DISPLAY)));

    return new Environment( globals );
}


