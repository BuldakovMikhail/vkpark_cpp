#include <iostream>
#include <sstream>

#include "operations_parsing.h"

bool isDouble(std::string const &s) {
    if (s.empty())
        return false;

    char *end;
    strtod(s.c_str(), &end);

    return *end == '\0';
}

bool isOperation(std::string const &s) {
    if (s == "+" || s == "-" || s == "/" || s == "round" || s == "floor")
        return true;

    return false;
}

bool isPriorityGreater(std::string const &l, std::string const &r) {
    if (l == "/" and r != "/")
        return true;
    else if ((l == "round" || l == "floor") && (r == "+" || r == "-"))
        return true;

    return false;
}
