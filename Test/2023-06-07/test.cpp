#include <bits/stdc++.h>
using namespace std;
int main() {
    int a, b;
    cin >> a >> b;
    int n = log(b) / log(2) + 1, cnt = 0;
    for (int i = n - 1, base = 1 << (n - 1); i >= 0; i--, base >>=) {
        if (a >> i & 1) continue;
        if (cnt + base <= b) cnt += base;
    }
    cout << a + cnt << endl;
    system("pause");
    return 0;
}