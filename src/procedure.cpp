#include "procedure.h"
#include <sstream>

using namespace std;

ostream & operator << (ostream &output, const Procedure *expression) {

    output << "<PROCEDURE>";
    return output;
}

