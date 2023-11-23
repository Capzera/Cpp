#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, q, l, r, a;
    char x;
    cin >> n >> q;
    set<int> ret;
    for (int i = 0; i < n; i++) {
        cin >> a >> x >> l >> x >> r;
        ret.emplace((r - l) / a);
    }
    vector<int> num(ret.begin(), ret.end());
    while (q--) {
        cin >> l >> r;
        int x = lower_bound(num.begin(), num.end(), l) - num.begin();
        int y = lower_bound(num.begin(), num.end(), r + 1) - num.begin();
        cout << y - x << endl;
    }
    system("pause");
    return 0;
}