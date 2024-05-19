#include <bits/stdc++.h>
using namespace std;
int t, n, k;
int solve() {
    cin >> n >> k;
    vector<int> a(n), b(n), h(n);
    iota(h.begin(), h.end(), 0);
    for (auto& x : a) cin >> x;
    for (auto& x : b) cin >> x;
    sort(h.begin(), h.end(), [&](auto& x, auto& y) { return a[x] < a[y]; });
    for (auto& i : h) {
        if (k < a[i]) return k;
        k += b[i];
    }
    return k;
}
int main() {
    cin >> t;
    while (t--) {
        cout << solve() << endl;
    }
    system("pause");
    return 0;
}
