#include <bits/stdc++.h>
using namespace std;
long long n, k, l, r, ans;
long long a[200010];

bool check(long long x) {
    long long cnt = 0;
    for (int i = n / 2; i < n; i++) {
        if (a[i] >= x) break;
        cnt += x - a[i];
    }
    return cnt <= k;
}

int main() {
    cin >> n >> k;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    sort(a, a + n);
    l = a[n / 2], r = l + k + 1, ans = l;
    while (l < r) {
        long long x = l + (r - l) / 2;
        if (check(x)) {
            ans = x;
            l = x + 1;
        }
        else r = x;
    }
    cout << ans << endl;
    return 0;
}