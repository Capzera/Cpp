#include <bits/stdc++.h>
using namespace std;
int main() {
    int t;
    cin >> t;
    while (t--) {
        int n, l = INT_MIN, r = INT_MAX, op, x, cnt = 0;
        unordered_set<int> s;
        cin >> n;
        while (n--) {
            cin >> op >> x;
            if (op == 1) l = max(l, x);
            if (op == 2) r = min(r, x);
            if (op == 3) s.emplace(x);
        }
        for (auto& xx : s) {
            cnt += xx >= l && xx <= r;
        }
        cout << max(0, r - l + 1 - cnt) << endl;
    }
    system("pause");
    return 0;
}