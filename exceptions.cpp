#include "exceptions.h"

using namespace std;

ostream & operator << (ostream &output, const SchemerException &exception) {

    output << exception.errorMessage;

    if (exception.line != 0 && exception.column != 0) {
        output << ": at line ";
        output << exception.line;
        output << ", column ";
        output << exception.column;
    }

    return output;
}

