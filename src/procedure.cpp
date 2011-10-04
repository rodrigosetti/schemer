#include "procedure.h"
#include <sstream>

using namespace std;

ostream & Procedure::print(ostream &output) const {

    output << "<PROCEDURE " << formalParameters.size() << '>';
    return output;
}

