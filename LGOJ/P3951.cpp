#include <bits/stdc++.h>
using namespace std;
int main() {
    long long a, b;
    cin >> a >> b;
    if (a < b) swap(a, b);
    long long sum = 1LL * a * b, i = sum - 1, j;
    for (; i >= b; i--) {
        int flag = 1;
        for (j = 0; flag && a * j <= i && j < b; j++) {
            int rem = i - a * j;
            if (rem % b == 0) flag = 0;
        }
        if (flag) break;
    }
    cout << i << endl;
    system("pause");
    return 0;
}