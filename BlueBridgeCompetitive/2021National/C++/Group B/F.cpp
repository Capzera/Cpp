#include <bits/stdc++.h>
#define ll long long
using namespace std;
int main() {
    ll t, x, y;
    cin >> t;
    auto solve = [](ll n) -> ll {
        if (!n) return 0;
        ll l = 0, r = 1414215;
        while (l < r) {
            ll mid = l + (r - l) / 2, sum = 1LL * mid * (mid + 1) / 2;
            if (sum > n) r = mid;
            else l = mid + 1;
        }
        ll ans = 1LL * l * (l - 1) / 2 * (l + 1) / 3;
        n -= 1LL * l * (l - 1) / 2;
        return ans + 1LL * n * (n + 1) / 2;
    };
    while (t--){
        cin >> x >> y;
        cout << solve(y) - solve(x - 1) << endl;
    }
    system("pause");
    return 0;
}
