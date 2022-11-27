#include <iostream>
#include <math.h>
using namespace std;
int main() {
    int x, fib0 = 0, fib1 = 1, fib;
    cin >> x;
    while (fib <= x) {
        fib = fib0 + fib1;
        fib0 = fib1;
        fib1 = fib;
    }
    cout << min(fib - x, x - fib0) << endl;
    return 0;
}