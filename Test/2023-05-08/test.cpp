#include "test.h"

test::test() {

}

test::test(int a, int b) {
    this->v1 = a;
    this->v2 = b;
}

int test::solve() {
    return v1 * v1 + v2 * v2;
}

test::~test() {
    
}