#include <iostream>

#include "operations_parsing.h"

std::unordered_map<std::string, int> getOperations() {
    std::unordered_map<std::string, int> ops = {{"+",     0},
                                                {"-",     0},
                                                {"round", 1},
                                                {"floor", 1},
                                                {"/",     2}};

    return ops;
}

bool isDouble(const std::string &s) {
    if (s.empty())
        return false;

    char *end;
    strtod(s.c_str(), &end);

    return *end == '\0';
}

bool isOperation(const std::string &s, const std::unordered_map<std::string, int> &map) {
    return map.find(s) != map.end();
}

bool isPriorityGreater(const std::string &l, const std::string &r, const std::unordered_map<std::string, int> &map) {
    return map.at(l) > map.at(r);
}
