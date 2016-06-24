#include "register.h"

string Register::getContentType() {
    if (value.find("$") != string::npos)
        return "IV";
    else if (value[0] > 48 && value[0] < 57)
        return "constant";
    else
        return "variable";
}
