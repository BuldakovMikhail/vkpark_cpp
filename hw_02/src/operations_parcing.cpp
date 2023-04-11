#include <iostream>
#include <sstream>

#include "operations_parsing.h"

bool isDouble(std::string const &s) {
    try {
        size_t idx = 0;
        std::stod(s, &idx);
        return idx == s.size();
    } catch (...) {
        return false;
    }
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
