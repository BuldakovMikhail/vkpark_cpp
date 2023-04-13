#include <iostream>
#include <stack>
#include <queue>

#include <sstream>


#include "operations_parsing.h"
#include "Calculatable.h"

#include "expression_handling.h"

std::string infixToPostfix(std::string const &infix, bool (*isOperation)(std::string const &),
                           bool (*isPriorityGreater)(std::string const &, std::string const &)) {
    std::string postfix;

    std::istringstream strStream(infix);
    std::string token;

    std::queue<std::string> queue;
    std::stack<std::string> stack;

    while (std::getline(strStream, token, ' ')) {
        if (isDouble(token)) {
            queue.push(token);
        } else if (isOperation(token)) {
            if (stack.empty() || stack.top() == "(")
                stack.push(token);
            else if (isPriorityGreater(token, stack.top()))
                stack.push(token);
            else {
                while (!stack.empty() && stack.top() != "(" && !isPriorityGreater(token, stack.top())) {
                    queue.push(stack.top());
                    stack.pop();
                }
                stack.push(token);
            }

        } else if (token == "(") {
            stack.push(token);
        } else if (token == ")") {
            while (!stack.empty() && stack.top() != "(") {
                queue.push(stack.top());
                stack.pop();
            }
            if (!stack.empty())
                stack.pop();
        } else {
            return {};
        }
    }
    while (!stack.empty()) {
        queue.push(stack.top());
        stack.pop();
    }

    while (!queue.empty()) {
        postfix += queue.front() + " ";
        queue.pop();
    }

    return postfix;
}

ICalculatableUPtr buildTree(std::string const &postfix) {

    std::stack<ICalculatableUPtr> stack;

    std::istringstream strStream(postfix);
    std::string token;

    while (std::getline(strStream, token, ' ')) {
        if (isOperation(token)) {
            
            auto x = std::move(stack.top());
            stack.pop();

            if (token == "+") {
                auto y = std::move(stack.top());
                stack.pop();

                stack.push(std::unique_ptr<ICalculatable>(new Plus(std::move(x), std::move(y))));
            } else if (token == "-") {
                auto y = std::move(stack.top());
                stack.pop();

                stack.push(std::unique_ptr<ICalculatable>(new Minus(std::move(x), std::move(y))));
            } else if (token == "/") {
                auto y = std::move(stack.top());
                stack.pop();

                stack.push(std::unique_ptr<ICalculatable>(new Divide(std::move(x), std::move(y))));
            } else if (token == "round") {
                stack.push(std::unique_ptr<ICalculatable>(new Round(std::move(x))));
            } else if (token == "floor") {
                stack.push(std::unique_ptr<ICalculatable>(new Floor(std::move(x))));
            }
        } else {
            try {
                double c = std::stod(token);
                stack.push(std::unique_ptr<ICalculatable>(new Num(c)));
            } catch (...) { break; }
        }
    }

    return std::move(stack.top());
}

bool expresionValidate(std::string const &postfix, bool (*isOperation)(std::string const &)) {
    std::istringstream strStream(postfix);
    std::string token;

    bool isCorrect = true;
    while (isCorrect && std::getline(strStream, token, ' ')) {
        if (!(isDouble(token) || isOperation(token)))
            isCorrect = false;
    }

    return isCorrect;
}