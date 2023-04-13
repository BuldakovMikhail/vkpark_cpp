#include <iostream>
#include <string>

#include "Calculatable.h"

#ifndef __EXPRESSION_HANDLING_H__
#define __EXPRESSION_HANDLING_H__

std::string infixToPostfix(std::string const &infix, const std::unordered_map<std::string, int> &operations);

ICalculatableUPtr buildTree(std::string const &postfix, const std::unordered_map<std::string, int> &operations);

bool expresionValidate(std::string const &postfix, const std::unordered_map<std::string, int> &operations);

#endif //HW_02_EXPRESSION_HANDLING_H
