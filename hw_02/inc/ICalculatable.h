#ifndef __ICALCULATABLE_H__
#define __ICALCULATABLE_H__

class ICalculatable {
public:
    virtual ~ICalculatable() {};

    virtual double Calculate() const = 0;
};

#endif //__ICALCULATABLE_H__
