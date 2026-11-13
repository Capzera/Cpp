#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, x;
    cin >> n;
    for (int i = n; i >= 0; i--) {
        cin >> x;
        if (!x) continue;
        if (i != n && x > 0) cout << '+';
        if (!i || abs(x) != 1) cout << x;
        if (x == -1 && i) cout << '-';
        if (i) cout << 'x';
        if (i > 1) cout << "^" << i;
    }
    system("pause");
    return 0;
}   