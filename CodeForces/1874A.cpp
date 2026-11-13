#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> a(n), b(m);
    for (auto& x : a) cin >> x;
    for (auto& x : b) cin >> x;
    int p = k % 2 ? 1 : 2;
    for (int i = 0; i < p; i++) {
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        if (a[0] < b.back()) swap(a[0], b.back());
        swap(a, b);
    }
    if (p == 1) swap(a, b);
    cout << accumulate(a.begin(), a.end(), 0LL) << endl;
    system("pause");
    return 0;
}