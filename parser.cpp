#include "parser.h"
#include <sstream>

using namespace std;

Token *next_token(list<Token*> &tokens) {

    Token *token = tokens.front();
    tokens.pop_front(); 
    return token;
}

Expression* Expression::parse(list<Token*> &tokens) throw (SchemerException) {

    if ( tokens.empty() )
        throw SchemerException("Unexpected end of input.");

    Token *token;

    // consume the next token
    token = next_token(tokens);

    if (token->type == TOK_OPEN) {

        // checks but dont consume next token
        token = tokens.front();

        switch (token->type) {
            case TOK_SYMBOL:
                return ApplicationExpression::parse(tokens);
            case TOK_RESERVED:
                switch (((ReservedWordToken*)token)->reservedWord) {
                    case RES_LAMBDA :
                        return LambdaExpression::parse(tokens);
                    case RES_DEFINE :
                        return DefineExpression::parse(tokens);
                    case RES_IF :
                        return IfExpression::parse(tokens);
                    case RES_QUOTE :
                        return QuoteExpression::parse(tokens);
                    case RES_BEGIN :
                        return BeginExpression::parse(tokens);
                }
            default:
                throw SchemerException("Unexpected symbol", token->line, token->column);
        }
    }
    else {
        return new Atom(token);
    }
}

/************************************/

ostream & operator << (ostream &output, const Atom &expression) {

    output << *(expression.token);
    return output;
}

ostream & operator << (ostream &output, const DefineExpression &expression) {

    output << "(DEFINE " << expression.defined << ')';
    return output;
}

ostream & operator << (ostream &output, const LambdaExpression &expression) {

    output << "(LAMBDA " << '(';

    bool separate = false;
    for (list<SymbolToken*>::const_iterator i = expression.formalParameters.begin();
         i != expression.formalParameters.end();
         i++) {
        output << **i;
        if (separate) output << ' ';
        separate = true;
    }
    output << ") " << expression.lambdaExpression << ')';
    return output;
}

ostream & operator << (ostream &output, const IfExpression &expression) {

    output << "(IF " << expression.condition 
           << ' ' << expression.conseq 
           << expression.otherwise << ')';
    return output;
}

ostream & operator << (ostream &output, const QuoteExpression &expression) {

    output << "(QUOTE " << expression.quoted << ')';
    return output;
}

ostream & operator << (ostream &output, const BeginExpression &expression) {

    output << "(BEGIN ";
    bool separate = false;
    for (list<Expression*>::const_iterator i = expression.expressions.begin();
         i != expression.expressions.end();
         i++) {
        output << *i;
        if (separate) output << ' ';
        separate = true;
    }
    output << ')';
    return output;
}

ostream & operator << (ostream &output, const ApplicationExpression &expression) {

    output << '(' << expression.function << ' ';
    bool separate = false;
    for (list<Expression*>::const_iterator i = expression.arguments.begin();
         i != expression.arguments.end();
         i++) {
        output << *i;
        if (separate) output << ' ';
        separate = true;
    }
    output << ')';
    return output;
}

ostream & operator << (ostream &output, const Procedure &expression) {

    output << "<PROCEDURE>";
    return output;
}

ostream & operator << (ostream &output, const BuiltInProcedure &expression) {

    output << "<BUILTIN>";
    return output;
}

