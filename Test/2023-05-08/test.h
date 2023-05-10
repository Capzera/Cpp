#ifndef _TEST_H_
#define _TEST_

#include "common.h"

class test {
private:
    int v1, v2;
public:
    test();
    test(int, int);
    int solve();
    ~test();
};

#endif