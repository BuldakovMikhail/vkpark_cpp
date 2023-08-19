#include <iostream>
#include <unordered_map>

#ifndef __OPERATIONS_PARSING_H__
#define __OPERATIONS_PARSING_H__

std::unordered_map<std::string, int> getOperations();

bool isDouble(const std::string &s);

bool isOperation(const std::string &s, const std::unordered_map<std::string, int> &map);

bool isPriorityGreater(const std::string &l, const std::string &r, const std::unordered_map<std::string, int> &map);

#endif
