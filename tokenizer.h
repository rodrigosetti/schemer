#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <istream>
#include <ostream>
#include <string>
#include <list>

#define MAX_SYMBOL_LENGHT 128

typedef enum { TOK_SYMBOL,
               TOK_OPEN,
               TOK_CLOSE } TokenType;

class SchemerException {

    public:
        unsigned int line;
        unsigned int column;
        std::string errorMessage;

        SchemerException(const std::string errorMessage,
                         const unsigned int line = 0,
                         const unsigned int column = 0) {
            this->errorMessage = errorMessage;
            this->line = line;
            this->line = column;
        }

        SchemerException(const char *errorMessage,
                         const unsigned int line = 0,
                         const unsigned int column = 0) {
            this->errorMessage = std::string(errorMessage);
            this->line = line;
            this->line = column;
        }
};

class Token {

    public:

        std::string symbolValue;
        TokenType type;
        unsigned int line;
        unsigned int column;
        bool quoted;

        Token ( const TokenType type,
                const unsigned int line,
                const unsigned int column,
                bool quoted=false,
                const std::string symbolValue = std::string());

        void print (std::ostream &output);

        static std::list<Token> tokenize (std::istream &stream) throw (SchemerException);
};

#endif

