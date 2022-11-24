#include <bits/stdc++.h>
using namespace std;
int main() {
    string table = "0123456789ABCDEF", ans = "";
    int m, n;
    cin >> m >> n;
    if (!m) cout << 0;
    if (m < 0) cout << "-";
    m = abs(m);
    while (m) {
        ans += table[m % n];
        m /= n;
    }
    reverse(ans.begin(), ans.end());
    cout << ans << endl;
    system("pause");
    return 0;
}