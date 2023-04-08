#include <iostream>
#include <string>

#include "Calculatable.h"

#ifndef __EXPRESSION_HANDLING_H__
#define __EXPRESSION_HANDLING_H__

std::string infixToPostfix(std::string const &infix, bool (*isOperation)(std::string const &),
                           bool (*isPriorityGreater)(std::string const &, std::string const &));

ICalculatableUPtr buildTree(std::string const &postfix);

bool expresionValidate(std::string const &postfix, bool (*isOperation)(std::string const &));

#endif //HW_02_EXPRESSION_HANDLING_H
