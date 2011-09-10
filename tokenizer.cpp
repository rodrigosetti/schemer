#include "tokenizer.h"

using namespace std;

Token::Token(const TokenType type,
             const unsigned int line,
             const unsigned int column,
             const bool quoted,
             const string symbolValue) {
    this->type = type;
    this->line = line;
    this->column = column;
    this->quoted = quoted;
    this->symbolValue = symbolValue;
}

typedef enum { ST_COMMENT,
               ST_WHITESPACE,
               ST_SYMBOL,
               ST_OPEN,
               ST_CLOSE,
               ST_QUOTE} TokenizerState;

list<Token> Token::tokenize(istream &stream) throw (SchemerException) {

    list<Token> tokens = list<Token>();
    char next;
    TokenizerState last_state = ST_WHITESPACE;
    TokenizerState cur_state;
    unsigned int line = 1;
    unsigned int column = 0;
    unsigned int s = 0;
    TokenType reserved_symbol;
    char symbol_buffer[MAX_SYMBOL_LENGHT];
    string symbol_str;
    bool quoted = false;

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
                if (quoted && last_state != ST_SYMBOL) {
                    throw SchemerException("Cannot quote before enclosing parenthesis", line, column);
                }
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
            case '\'':
                if (last_state != ST_SYMBOL)
                    quoted = false;
                else {
                    throw SchemerException("Illegal char \"'\" in token", line, column);
                }
                cur_state = ST_QUOTE;
                quoted = true;
                break;
            default:
                cur_state = ST_SYMBOL;
        }

        /* if reading a symbol is over, write it to a new token */
        if (last_state == ST_SYMBOL && cur_state != ST_SYMBOL) {
            symbol_str = string(symbol_buffer, s);
            tokens.push_back(Token(TOK_SYMBOL, line, column, quoted, symbol_str));
            quoted = false;
            s = 0;
        }
        else if (cur_state == ST_SYMBOL) {

            symbol_buffer[s++] = next;

            if (s == MAX_SYMBOL_LENGHT) {
                throw SchemerException("Maximum symbol length violated.", line, column);
            }
        }

        /* adds apropriate token for opening or closing parenthesis */
        if (cur_state == ST_OPEN) {
            tokens.push_back(Token(TOK_OPEN, line, column, quoted));
            quoted = false;
        }
        else if (cur_state == ST_CLOSE) {
            tokens.push_back(Token(TOK_CLOSE, line, column, false));
        }

        last_state = cur_state;
    }

    return tokens;
}

void Token::print(ostream &output) {

    switch (type) {
        case TOK_SYMBOL:
            output << symbolValue;
            break;
        case TOK_OPEN:
            output << '(';
            break;
        case TOK_CLOSE:
            output << ')';
            break;
    }
}

