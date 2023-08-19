#include <iostream>
#include <string>

#include "Calculatable.h"

#ifndef __EXPRESSION_HANDLING_H__
#define __EXPRESSION_HANDLING_H__

std::string infixToPostfix(const std::string &infix, const std::unordered_map<std::string, int> &operations);

ICalculatableUPtr buildTree(const std::string &postfix, const std::unordered_map<std::string, int> &operations);

bool expresionValidate(const std::string &postfix, const std::unordered_map<std::string, int> &operations);

#endif //HW_02_EXPRESSION_HANDLING_H
