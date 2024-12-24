#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, k;
    cin >> n >> k;
    long long mx = pow(n, n), now = n, ans = 0;
    for (int i = n; i <= mx; i += 10) {
        if (i % k % n == 0) ans++;
    }
    cout << ans << endl;
    return 0;
}
