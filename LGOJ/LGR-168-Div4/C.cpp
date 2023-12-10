#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, k;
    cin >> n >> k;
    long long mx = pow(n, n), now = n, ans = 0;
    while (now <= mx) {
        if (now % k % n == 0) ans++;
        now += 10;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}