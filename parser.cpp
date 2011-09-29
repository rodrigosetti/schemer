#include "parser.h"
#include <sstream>

using namespace std;

Token *next_token(list<Token*> &tokens) {

    Token *token = tokens.front();
    tokens.pop_front(); 
    return token;
}

BeginExpression::~BeginExpression() {
    for (std::list<Expression*>::iterator i = expressions.begin();
         i != expressions.end();
         i++) {
        delete *i;
    }
}

ApplicationExpression::~ApplicationExpression() {
    for (std::list<Expression*>::iterator i = arguments.begin();
         i != arguments.end();
         i++) {
        delete *i;
    }
}

/**
 * About parsing:
 *
 * The Expression::parse is the entry point for parsing, and must be used
 * by the user of this lib.
 * the other parse methods should be called by this by the point it already
 * knows how to decide which parse method to call, hence, those secundary
 * parsing should expects just the extra tokens (i.e. the part which is mutable
 * among its type)
 */
Expression* Expression::parse(list<Token*> &tokens) throw (SchemerException) {

    if ( tokens.empty() )
        throw SchemerException("Unexpected end of input.");

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
                case RES_QUOTE :
                    return QuoteExpression::parse(tokens);
                case RES_BEGIN :
                    return BeginExpression::parse(tokens);
            }
        }
        else {
            // put back next token, because it will be used by application
            tokens.push_front(token);
            return ApplicationExpression::parse(tokens);
        }
    }
    else {
        return new Atom(token);
    }
}

Expression *ApplicationExpression::parse(list<Token*> &tokens) throw (SchemerException) {
    ApplicationExpression *expression = new ApplicationExpression();

    expression->function = Expression::parse(tokens);

    while (tokens.front()->type != TOK_CLOSE) {
        expression->arguments.push_back( Expression::parse(tokens) );
        if (tokens.empty()) {
            throw SchemerException("Unexpected end of input (expecting ')')");
        }
    }
    next_token(tokens);

    return expression;
}

Expression *LambdaExpression::parse(list<Token*> &tokens) throw (SchemerException) {
    Token *token;
    LambdaExpression *expression = new LambdaExpression();

    token = next_token(tokens);

    if (token->type != TOK_OPEN) {
        throw SchemerException("Expected (", token->line, token->column);
    }
    while (true) {
        token = next_token(tokens);
        if (token->type == TOK_CLOSE) {
            break;
        }
        else if (token->type == TOK_SYMBOL) {
            expression->formalParameters.push_back((SymbolToken*)token);
        }
        else {
            throw SchemerException("Unexpected token");
        }
    }
    expression->lambdaExpression = Expression::parse(tokens);

    token = next_token(tokens);
    if (token->type != TOK_CLOSE) {
        throw SchemerException("Expected )", token->line, token->column);
    }

    return expression;
}

Expression *DefineExpression::parse(list<Token*> &tokens) throw (SchemerException) {
    Token *token;
    DefineExpression *expression = new DefineExpression();

    token = next_token(tokens);
    if (token->type != TOK_SYMBOL) {
        throw SchemerException("Expected symbol");
    }

    expression->name = token;
    expression->defined = Expression::parse(tokens);

    token = next_token(tokens);
    if (token->type != TOK_CLOSE) {
        throw SchemerException("Expected )", token->line, token->column);
    }

    return expression;
}

Expression *IfExpression::parse(list<Token*> &tokens) throw (SchemerException) {
    Token *token;
    IfExpression *expression = new IfExpression();

    expression->condition = Expression::parse(tokens);
    expression->conseq = Expression::parse(tokens);
    expression->otherwise = Expression::parse(tokens);

    token = next_token(tokens);
    if (token->type != TOK_CLOSE) {
        throw SchemerException("Expected )", token->line, token->column);
    }

    return expression;
}

Expression *QuoteExpression::parse(list<Token*> &tokens) throw (SchemerException) {
    QuoteExpression *expression = new QuoteExpression();

    return expression;
}

Expression *BeginExpression::parse(list<Token*> &tokens) throw (SchemerException) {
    Token *token;
    BeginExpression *expression = new BeginExpression();

    while (true) {
        token = tokens.front();

        if (token && token->type == TOK_CLOSE) {
            break;
        } else {
            expression->expressions.push_back(Expression::parse(tokens));
        }
    }
    return expression;
}

/************************************/

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
        case EXP_QUOTE:
            output << (QuoteExpression*)expression;
            break;
        case EXP_APPLICATION:
            output << (ApplicationExpression*)expression;
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

ostream & operator << (ostream &output, const Atom *expression) {

    output << expression->token;
    return output;
}

ostream & operator << (ostream &output, const DefineExpression *expression) {

    output << "(DEFINE " << expression->name << " " << expression->defined << ')';
    return output;
}

ostream & operator << (ostream &output, const LambdaExpression *expression) {

    output << "(LAMBDA " << '(';

    bool separate = false;
    for (list<SymbolToken*>::const_iterator i = expression->formalParameters.begin();
         i != expression->formalParameters.end();
         i++) {
        if (separate) output << ' ';
        output << *i;
        separate = true;
    }
    output << ") " << expression->lambdaExpression << ')';
    return output;
}

ostream & operator << (ostream &output, const IfExpression *expression) {

    output << "(IF " << expression->condition 
           << ' ' << expression->conseq 
           << expression->otherwise << ')';
    return output;
}

ostream & operator << (ostream &output, const QuoteExpression *expression) {

    output << "(QUOTE " << expression->quoted << ')';
    return output;
}

ostream & operator << (ostream &output, const BeginExpression *expression) {

    output << "(BEGIN ";
    bool separate = false;
    for (list<Expression*>::const_iterator i = expression->expressions.begin();
         i != expression->expressions.end();
         i++) {
        output << *i;
        if (separate) output << ' ';
        separate = true;
    }
    output << ')';
    return output;
}

ostream & operator << (ostream &output, const ApplicationExpression *expression) {

    output << '(' << expression->function << ' ';
    bool separate = false;
    for (list<Expression*>::const_iterator i = expression->arguments.begin();
         i != expression->arguments.end();
         i++) {
        if (separate) output << ' ';
        output << *i;
        separate = true;
    }
    output << ')';
    return output;
}

ostream & operator << (ostream &output, const Procedure *expression) {

    output << "<PROCEDURE>";
    return output;
}

ostream & operator << (ostream &output, const BuiltInProcedure *expression) {

    output << "<BUILTIN>";
    return output;
}

