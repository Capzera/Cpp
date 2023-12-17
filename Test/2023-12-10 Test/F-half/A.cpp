#include <bits/stdc++.h>
using namespace std;
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int n, m, q, l, r;
    cin >> n >> m >> q;
    vector<int> sum(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> sum[i];
        sum[i] = sum[i - 1] + (__gcd(sum[i], q) == 1);
    }
    while (m--) {
        cin >> l >> r;
        cout << sum[r] - sum[l - 1] << endl;
    }
    system("pause");
    return 0;
}