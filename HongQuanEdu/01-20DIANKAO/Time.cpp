#include <bits/stdc++.h>
using namespace std;
int main() {
    int t;
    cin >> t;
    while (t--) {
        int n, x = 0, y = INT_MAX, l, r;
        cin >> n;
        while (n--) {
            cin >> l >> r;
            x = max(x, l);
            y = min(y, r);
        }
        if (x > y) cout << "NO" << endl;
        else cout << "YES" << endl;
    }
    return 0;
}