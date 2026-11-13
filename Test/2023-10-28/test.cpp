#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int a[N], b[N], c[N], cnt, cnt1;
int ans1, ans2;
int main() {
    int cnt = 0;
    while (cin >> a[cnt]) {
        cnt++;
    }
    for (int i = cnt - 1; i >= 0; i--) {
        int loc = upper_bound(c, c + ans1, a[i]) - c;
        if (loc >= ans1) c[ans1++] = a[i];
        else c[loc] = a[i];
    }
    for (int i = 0; i < cnt; i++) {
        int loc = lower_bound(b, b+ ans2, a[i]) - b;
        if (loc >= ans2) b[ans2++] = a[i];
        else b[loc] = a[i];
    }
    cout << ans1 << endl << ans2 << endl;
    return 0;
}