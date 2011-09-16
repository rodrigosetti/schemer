#include "tokenizer.h"
#include <sstream>

using namespace std;

Token::Token( const TokenType type,
              const unsigned int line,
              const unsigned int column) {
    this->type = type;
    this->column = column;
}

typedef enum { ST_COMMENT,
               ST_WHITESPACE,
               ST_SYMBOL,
               ST_OPEN,
               ST_CLOSE } TokenizerState;

list<Token*> Token::tokenize(istream &stream) throw (SchemerException) {

    list<Token*> tokens = list<Token*>();
    char next;
    TokenizerState last_state = ST_WHITESPACE;
    TokenizerState cur_state;
    unsigned int line = 1;
    unsigned int column = 0;
    unsigned int s = 0;
    static char symbol_buffer[MAX_SYMBOL_LENGHT];
    string symbol_str;

    while ( (next = stream.get()) != EOF) {

        /* count lines and columns */
        if (next == '\n') {
            line ++;
            column = 0;
        }
        else column ++;

        /* if state is comment and not end of line,
         * continue to the next char */
        if (last_state == ST_COMMENT && next != '\n')
            continue;

        switch (next) {
            case '(':
                cur_state = ST_OPEN;
                break;
            case ')':
                cur_state = ST_CLOSE;
                break;
            case '\n':
            case ' ':
            case '\t':
                cur_state = ST_WHITESPACE;
                break;
            case ';':
                cur_state = ST_COMMENT;
                break;
            default:
                cur_state = ST_SYMBOL;
        }

        /* if reading a symbol is over, write it to a new token */
        if (last_state == ST_SYMBOL && cur_state != ST_SYMBOL) {
            symbol_str = string(symbol_buffer, s);

            if (ReservedWordToken::match(symbol_str)) {
                tokens.push_back(new ReservedWordToken(symbol_str, line, column));
            }
            else if (FloatToken::match(symbol_str)) {
                tokens.push_back(new FloatToken(symbol_str, line, column));
            }
            else if (IntToken::match(symbol_str)) {
                tokens.push_back(new IntToken(symbol_str, line, column));
            }
            else {
                tokens.push_back(new SymbolToken(symbol_str, line, column));
            }
            s = 0; // start counting again
        }
        else if (cur_state == ST_SYMBOL) {

            symbol_buffer[s++] = next;

            if (s == MAX_SYMBOL_LENGHT) {
                throw SchemerException("Maximum symbol length violated.", line, column);
            }
        }

        /* adds apropriate token for opening or closing parenthesis */
        if (cur_state == ST_OPEN) {
            tokens.push_back(new Token(TOK_OPEN, line, column));
        }
        else if (cur_state == ST_CLOSE) {
            tokens.push_back(new Token(TOK_CLOSE, line, column));
        }

        last_state = cur_state;
    }

    return tokens;
}

void Token::destroy(list<Token*> &tokens) {

    while (!tokens.empty()) {
        delete tokens.front();
        tokens.pop_front();
    }
}

/*************************************/

SymbolToken::SymbolToken (const string &symbol,
                          const unsigned int line,
                          const unsigned int column) :
    Token(TOK_SYMBOL, line, column) {
    this->symbolValue = string(symbol);
}

/*************************************/

ReservedWordToken::ReservedWordToken (const string &symbol,
                                      const unsigned int line,
                                      const unsigned int column) :
    Token(TOK_RESERVED, line, column) {

    if (symbol == "lambda") reservedWord = RES_LAMBDA;
    else if (symbol == "define") reservedWord = RES_DEFINE;
    else if (symbol == "if") reservedWord = RES_IF;
    else if (symbol == "quote") reservedWord = RES_QUOTE;
    else if (symbol == "begin") reservedWord = RES_BEGIN;
    else
        throw SchemerException("Bad token allocation (reserved word)", line, column);
}

bool ReservedWordToken::match(const string &symbol) {

    if (symbol == "lambda") return true;
    else if (symbol == "define") return true;
    else if (symbol == "if") return true;
    else if (symbol == "quote") return true;
    else if (symbol == "begin") return true;
    else return false;
}

/*************************************/

FloatToken::FloatToken (const string &symbol,
                        const unsigned int line,
                        const unsigned int column) :
    Token(TOK_FLOAT, line, column) {

    istringstream test(symbol);
    test >> floatValue;
}

bool FloatToken::match(const string &symbol) {
    long double f;
    istringstream test(symbol);
    return (test >> f);
}

/*************************************/

IntToken::IntToken (const string &symbol,
                        const unsigned int line,
                        const unsigned int column) :
    Token(TOK_INT, line, column) {

    istringstream test(symbol);
    test >> intValue;
}

bool IntToken::match(const string &symbol) {
    long int f;
    istringstream test(symbol);
    return (test >> f);
}

/****************************************/

ostream& operator << (ostream &output, const Token &token) {

    switch (token.type) {
        case TOK_OPEN:
            output << '(';
            break;
        case TOK_CLOSE:
            output << ')';
            break;
    }

    return output;
}

ostream& operator << (ostream &output, const SymbolToken &token) {

    output << token.symbolValue;
    return output;
}

ostream& operator << (ostream &output, const ReservedWordToken &token) {
    switch (token.reservedWord) {
        case RES_DEFINE:
            output << "DEFINE";
            break;
        case RES_BEGIN:
            output << "BEGIN";
            break;
        case RES_IF:
            output << "IF";
            break;
        case RES_QUOTE:
            output << "QUOTE";
            break;
        case RES_LAMBDA:
            output << "LAMBDA";
            break;
    }
    return output;
}

ostream& operator << (ostream &output, const FloatToken &token) {
    output << token.floatValue;
    return output;
}

ostream& operator << (ostream &output, const IntToken &token) {
    output << token.intValue;
    return output;
}



