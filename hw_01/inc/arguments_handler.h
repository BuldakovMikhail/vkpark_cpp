#include <iostream>

#ifndef __ARGUMENTS_HANDLER_H__
#define __ARGUMENTS_HANDLER_H__

struct Arguments
{
    int runtime;
    std::string akas;
    std::string basics;
    std::string ratings;
    std::string data;
};

Arguments parseArguments(int argc, char const *argv[]);

#endif //__ARGUMENTS_HANDLER_H__
