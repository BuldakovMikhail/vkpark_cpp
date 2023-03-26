#include <sstream>

#include "string_processing.h"

void stringSplitByTabs(std::vector<std::string> &arr, std::string str)
{
    std::istringstream strStream(str);
    std::string token;

    while (getline(strStream, token, '\t'))
        arr.push_back(token);
}