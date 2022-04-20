#ifndef EXPRESS_H
#define EXPRESS_H

#include <iostream>
#include <string>

using namespace std;

class express {
public:
    char type;
    size_t key;

    virtual bool calc() = 0;
    virtual bool is_range(express *expr) = 0;
    virtual string print() const = 0;
};
#endif
