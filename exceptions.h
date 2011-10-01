#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <string>
#include <ostream>

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

std::ostream & operator << (std::ostream &output, const SchemerException &exception);

#endif

