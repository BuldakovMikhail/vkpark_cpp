#include <iostream>

#ifndef __OPERATIONS_PARSING_H__
#define __OPERATIONS_PARSING_H__

bool isDouble(const std::string &s);

bool isOperation(std::string const &s);

bool isPriorityGreater(std::string const &l, std::string const &r);

#endif
