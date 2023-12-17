#include <bits/stdc++.h>
using namespace std;
int main() {
    int m, n;
    cin >> m >> n;
    if (m > 4 && n > 4) {
        long long ans = 1ll * 8 * m * n - 12 * m - 12 * n + 16;
        cout << ans << endl;
    }
    else {
        if (m * n == 6) cout << 4 << endl;
        else if (m * n == 9) cout << 16 << endl;
        else if (m * n == 12) cout << 30 << endl;
        else if (m * n == 16) cout << 48 << endl;
        else cout << 0 << endl;
    }
    return 0;
}
