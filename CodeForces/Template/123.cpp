#include <bits/stdc++.h>
using namespace std;
int main() {
    int  t = 1, n, l, r;
    cin >> t;
    while (t--) {
        cin >> n;
        int ansl = INT_MIN, ansr = INT_MAX, flag = 0;
        for (int i = 0; i < n; i++) {
            cin >> l >> r;
            if (r < ansl || l > ansr) flag = 1;
            ansl = max(ansl, l), ansr = min(ansr, r);
        }
        if (flag) cout << "NO" << endl;
        else cout << "YES" << endl;
    }
    system("pause");
    return 0;
}