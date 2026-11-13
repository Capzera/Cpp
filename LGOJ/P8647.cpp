#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
int h[N], w[N], n, k;
int check(int x) {
    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans += (h[i] / x) * (w[i] / x);
    }
    return ans >= k;
}
int main() {
    cin >> n >> k;
    for (int i = 0; i < n ; i++) cin >> h[i] >> w[i];
    int l = 0, r = 1e5 + 1;
    while (r - l > 1) {
        int mid = l + (r - l) / 2;
        if (check(mid)) l = mid;
        else r = mid;
    }
    cout << l << endl;
    system("pause");
    return 0;
}