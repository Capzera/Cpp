#include <bits/stdc++.h>
using namespace std;

int solve() {
    int n, sum = 0;
    cin >> n;
    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++) cin >> a[i] >> b[i];
    for (int i = 0; i < n; i++) {
        if (sum < a[i]) return 0;
        sum += b[i] - a[i];
    }
    return n + 1;
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int t;
    cin >> t;
    while (t--) cout << solve() << endl;
    system("pause");
    return 0;
}