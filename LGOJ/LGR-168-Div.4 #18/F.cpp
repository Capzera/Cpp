#include <bits/stdc++.h>
using namespace std;
int main() {
    long long n, x, ans = 0, now, pre = 0;
    cin >> n;
    for (int i = 0; i <= n; i++) {
        cin >> now;
        ans += (i && 1ll * now * pre < 0);
        pre = now;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}