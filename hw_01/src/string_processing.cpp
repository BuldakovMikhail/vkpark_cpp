#include <sstream>

#include "string_processing.h"

std::vector<std::string> stringSplitByTabs(std::string str)
{
    std::vector<std::string> arr;

    std::istringstream strStream(str);
    std::string token;

    while (getline(strStream, token, '\t'))
        arr.push_back(token);

    return arr;
}
