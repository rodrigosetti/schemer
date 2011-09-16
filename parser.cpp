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

    return output;
}

ostream & operator << (ostream &output, const DefineExpression &expression) {

    return output;
}

ostream & operator << (ostream &output, const LambdaExpression &expression) {

    return output;
}

ostream & operator << (ostream &output, const IfExpression &expression) {

    return output;
}

ostream & operator << (ostream &output, const BeginExpression &expression) {

    return output;
}

ostream & operator << (ostream &output, const ApplicationExpression &expression) {

    return output;
}

ostream & operator << (ostream &output, const Procedure &expression) {

    return output;
}

ostream & operator << (ostream &output, const BuiltInProcedure &expression) {

    return output;
}

