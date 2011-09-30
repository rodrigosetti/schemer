#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include "globals.h"
#include <istream>
#include <ostream>
#include <list>

#define MAX_SYMBOL_LENGHT 128

typedef enum { TOK_SYMBOL,
               TOK_RESERVED,
               TOK_OPEN,
               TOK_BOOL,
               TOK_NIL,
               TOK_FLOAT,
               TOK_INT,
               TOK_CLOSE } TokenType;

typedef enum { RES_LAMBDA,
               RES_DEFINE,
               RES_COND,
               RES_QUOTE,
               RES_BEGIN } ReservedWordType;


class Token {

    public:

        TokenType type;
        unsigned int line;
        unsigned int column;

        Token ( const TokenType type,
                const unsigned int line,
                const unsigned int column);

        static std::list<Token*> tokenize (std::istream &stream) throw (SchemerException);
        static void destroy(std::list<Token*> &tokens);
};

class SymbolToken : public Token {

    public:

        std::string symbolValue;

        SymbolToken (const std::string &symbol,
                     const unsigned int line,
                     const unsigned int column);

        static bool match(const std::string &symbol);
};

class BoolToken : public Token {

    public:

        bool boolValue;

        BoolToken (const std::string &symbol,
                   const unsigned int line,
                   const unsigned int column);

        static bool match(const std::string &symbol);
};

class NilToken : public Token {

    public:

        NilToken () : Token (TOK_NIL, 0, 0) {}
        NilToken (const unsigned int line,
                  const unsigned int column) : Token(TOK_NIL, line, column) {}

        static bool match(const std::string &symbol);
};

class ReservedWordToken : public Token {

    public:

        ReservedWordType reservedWord;

        ReservedWordToken (const std::string &symbol,
                           const unsigned int line,
                           const unsigned int column);

        static bool match(const std::string &symbol);
};

class FloatToken : public Token {

    public:

        long double floatValue;

        FloatToken (const std::string &symbol,
                    const unsigned int line,
                    const unsigned int column);
        FloatToken (const long double value);

        static bool match(const std::string &symbol);
};

class IntToken : public Token {

    public:

        long int intValue;

        IntToken (const std::string &symbol,
                  const unsigned int line,
                  const unsigned int column);
        IntToken (const long int value);

        static bool match(const std::string &symbol);
};

std::ostream & operator << (std::ostream &output, const Token *token);

#endif

