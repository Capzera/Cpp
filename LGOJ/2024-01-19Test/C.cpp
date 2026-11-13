#include <bits/stdc++.h>
using namespace std;
long long n, k, l, r;
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
    l = 0, r = a[n / 2] + k + 1;
    while (r - l != 1) {
        long long x = l + (r - l) / 2;
        if (check(x)) {
            l = x;
        }
        else r = x;
    }
    cout << l << endl;
    return 0;
}