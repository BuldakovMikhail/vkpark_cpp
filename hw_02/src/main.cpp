#include <iostream>
#include <string>
#include <unordered_map>

#include "expression_handling.h"
#include "operations_parsing.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Передано пустое выражение" << std::endl;
        return -1;
    }

    auto map = getOperations();

    std::string postfix = infixToPostfix(argv[1], map);

    if (postfix.empty() || !expresionValidate(postfix, map)) {
        std::cerr << "Введено неверное выражение" << std::endl;
        return -1;
    }

    auto x = buildTree(postfix, map);

    if (!x) {
        std::cerr << "Ошибка в данных" << std::endl;
        return -1;
    }

    try {
        std::cout << x->Calculate() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
