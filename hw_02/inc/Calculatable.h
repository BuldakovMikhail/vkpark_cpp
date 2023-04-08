#include <iostream>
#include <memory>
#include <cmath>
#include <stdexcept>

#include "ICalculatable.h"

#ifndef __CALCULATABLE_H__
#define __CALCULATABLE_H__

using ICalculatableUPtr = std::unique_ptr<ICalculatable>;

class Plus : public ICalculatable {
public:
    Plus(ICalculatableUPtr left, ICalculatableUPtr right) {
        l.swap(left);
        r.swap(right);
    };

    double Calculate() const override {
        return l->Calculate() + r->Calculate();
    }

private:
    ICalculatableUPtr l;
    ICalculatableUPtr r;
};


class Minus : public ICalculatable {
public:
    Minus(ICalculatableUPtr right, ICalculatableUPtr left) {
        l.swap(left);
        r.swap(right);
    };

    double Calculate() const override {
        return l->Calculate() - r->Calculate();
    }

private:
    ICalculatableUPtr l;
    ICalculatableUPtr r;
};

class Divide : public ICalculatable {
public:
    Divide(ICalculatableUPtr right, ICalculatableUPtr left) {
        l.swap(left);
        r.swap(right);
    };

    double Calculate() const override {
        double val = r->Calculate();
        if (val == 0) throw std::runtime_error("Деление на 0");
        return l->Calculate() / val;
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

    double Calculate() const override {
        return round(r->Calculate());
    }

private:
    ICalculatableUPtr r;
};

class Num : public ICalculatable {
public:
    explicit Num(double data) : data_(data) {};

    double Calculate() const override {
        return data_;
    }

private:
    double data_;
};

#endif //__CALCULATABLE_H__
