#include <iostream>
#include <stack>
#include <queue>
#include <unordered_map>

#include <sstream>


#include "operations_parsing.h"
#include "Calculatable.h"

#include "expression_handling.h"

std::string infixToPostfix(const std::string &infix, const std::unordered_map<std::string, int> &operations) {
    std::string postfix;

    std::istringstream strStream(infix);
    std::string token;

    std::queue<std::string> queue;
    std::stack<std::string> stack;

    while (std::getline(strStream, token, ' ')) {
        if (isDouble(token)) {
            queue.push(token);

        } else if (isOperation(token, operations)) {
            if (!(stack.empty() ||
                  stack.top() == "(" ||
                  isPriorityGreater(token, stack.top(), operations))) {

                while (!stack.empty() && stack.top() != "(" &&
                       !isPriorityGreater(token, stack.top(), operations)) {
                    queue.push(stack.top());
                    stack.pop();
                }
            }
            stack.push(token);

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

ICalculatableUPtr buildTree(const std::string &postfix, const std::unordered_map<std::string, int> &operations) {

    std::stack<ICalculatableUPtr> stack;

    std::istringstream strStream(postfix);
    std::string token;

    while (std::getline(strStream, token, ' ')) {
        if (isOperation(token, operations)) {

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
            } catch (const std::exception &e) { return nullptr; }
        }
    }

    return std::move(stack.top());
}

bool expresionValidate(const std::string &postfix, const std::unordered_map<std::string, int> &operations) {
    std::istringstream strStream(postfix);
    std::string token;

    bool isCorrect = true;
    while (isCorrect && std::getline(strStream, token, ' ')) {
        if (!(isDouble(token) || isOperation(token, operations)))
            isCorrect = false;
    }

    return isCorrect;
}
