#include <bits/stdc++.h>
using namespace std;
int solve() {
    int x, n, pos = 0, neg = 0, flag = 0, ans = 0;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x;
        if (!x) flag = 1;
        if (x > 0) pos ^= 1;
        else {
            neg ^= 1;
            ans = i + 1;
        }
    }
    if (flag || neg & 1) return 0;
    if (!neg) return 1;
    return ans;
}
int main() {
    int t = 1;
    cin >> t;
    while (t--) {
        int k = solve();
        if (!k) {
            cout << 0 << endl;
            continue;
        }
        printf("1\n%d 0\n", k);
    }
    system("pause");
    return 0;
}