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
               TOK_FLOAT,
               TOK_INT,
               TOK_CLOSE } TokenType;

typedef enum { RES_LAMBDA,
               RES_DEFINE,
               RES_IF,
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

        static bool match(const std::string &symbol);
};

class IntToken : public Token {

    public:

        long int intValue;

        IntToken (const std::string &symbol,
                  const unsigned int line,
                  const unsigned int column);

        static bool match(const std::string &symbol);
};

std::ostream & operator << (std::ostream &output, const Token &token);
std::ostream & operator << (std::ostream &output, const SymbolToken &token);
std::ostream & operator << (std::ostream &output, const ReservedWordToken &token);
std::ostream & operator << (std::ostream &output, const FloatToken &token);
std::ostream & operator << (std::ostream &output, const IntToken &token);

#endif

