#include <iostream>

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

class arguments_exception : public std::exception {
private:
    const char * message;

public:
    arguments_exception(const char * msg) : message(msg) {}
    const char * what () {
        return message;
    }
};

#endif //HW_01_EXCEPTION_H
