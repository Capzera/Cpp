#include <bits/stdc++.h>
using namespace std;
int main() {
    long long n;
    cin >> n;
    long long x = n, xin = 0;
    while (n > 4) {
        xin = n / 5 * 2;
        x += xin;
        n = n % 5 + xin;
    }
    cout << x << endl;
    system("pause");
    return 0;
}