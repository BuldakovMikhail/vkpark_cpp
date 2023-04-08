#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
#include <string>
#include <memory>
#include <cmath>

class ICalculatable {
public:
    virtual ~ICalculatable() {};

    virtual double Calculate() const = 0;
};

bool isDouble(const std::string &s) {
    std::istringstream iss(s);
    double d;
    return iss >> d >> std::ws && iss.eof();
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

std::string infixToPostfix(std::string const &infix) {
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

using ICalculatableUPtr = std::unique_ptr<ICalculatable>;

class Plus : public ICalculatable {
public:
    Plus(ICalculatableUPtr left, ICalculatableUPtr right) {
        l.swap(left);
        r.swap(right);
    };

    //~Plus() = default;

    double Calculate() const override {
        return l->Calculate() + r->Calculate();
    }

private:
    ICalculatableUPtr l;
    ICalculatableUPtr r;
};

class Minus : public ICalculatable {
public:
    Minus(ICalculatableUPtr left, ICalculatableUPtr right) {
        l.swap(left);
        r.swap(right);
    };

    //~Minus() = default;

    double Calculate() const override {
        return r->Calculate() - l->Calculate();
    }

private:
    ICalculatableUPtr l;
    ICalculatableUPtr r;
};

class Divide : public ICalculatable {
public:
    Divide(ICalculatableUPtr left, ICalculatableUPtr right) {
        l.swap(left);
        r.swap(right);
    };

    //~Divide() = default;

    double Calculate() const override {
        return r->Calculate() / l->Calculate();
    };

private:
    ICalculatableUPtr l;
    ICalculatableUPtr r;
};

class Floor : public ICalculatable {
public:
    explicit Floor(ICalculatableUPtr child) {
        r.swap(child);
    }

    //~Floor() = default;

    double Calculate() const override {
        return floor(r->Calculate());
    };

private:
    ICalculatableUPtr r;
};

class Round : public ICalculatable {
public:
    explicit Round(ICalculatableUPtr child) {
        r.swap(child);
    };

    //~Round() = default;

    double Calculate() const override {
        return round(r->Calculate());
    }

private:
    ICalculatableUPtr r;
};

class Num : public ICalculatable {
public:
    explicit Num(double data) : data_(data) {};

    //~Num() = default;

    double Calculate() const override {
        return data_;
    }

private:
    double data_;
};

ICalculatableUPtr buildTree(std::string &postfix) {

    std::stack<ICalculatableUPtr> stack;

    std::istringstream strStream(postfix);
    std::string token;

    while (std::getline(strStream, token, ' ')) {
        if (isOperation(token)) {
            if (token == "+") {
                auto x = std::move(stack.top());
                stack.pop();

                auto y = std::move(stack.top());
                stack.pop();

                stack.push(std::unique_ptr<ICalculatable>(new Plus(std::move(x), std::move(y))));
            } else if (token == "-") {
                auto x = std::move(stack.top());
                stack.pop();

                auto y = std::move(stack.top());
                stack.pop();

                stack.push(std::unique_ptr<ICalculatable>(new Minus(std::move(x), std::move(y))));
            } else if (token == "/") {
                auto x = std::move(stack.top());
                stack.pop();

                auto y = std::move(stack.top());
                stack.pop();

                stack.push(std::unique_ptr<ICalculatable>(new Divide(std::move(x), std::move(y))));
            } else if (token == "round") {
                auto x = std::move(stack.top());
                stack.pop();

                stack.push(std::unique_ptr<ICalculatable>(new Round(std::move(x))));
            } else if (token == "floor") {
                auto x = std::move(stack.top());
                stack.pop();

                stack.push(std::unique_ptr<ICalculatable>(new Floor(std::move(x))));
            }
        } else {
            double c = std::stod(token);
            stack.push(std::unique_ptr<ICalculatable>(new Num(c)));
        }
    }

    return std::move(stack.top());
}

int main(int argc, char *argv[]) {
    std::string postfix = infixToPostfix(argv[1]);
    auto x = buildTree(postfix);

    std::cout << x->Calculate() << std::endl;

    return 0;
}


