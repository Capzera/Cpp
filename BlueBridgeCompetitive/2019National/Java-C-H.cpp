#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define vi vector<int>
#define vll vector<long long>
#define vvll vector<vector<long long>>
template <class T>
void read(T &a, T &b) {
    cin >> a >> b;
}
template <class T>
void print(T n) {
    cout << n << endl;
}
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int m, n, len;
    read(m, n);
    auto check = [&](int mask) ->bool {
        for (int i = 0; i <= n - 3; i++) {
            if ((mask >> i & 1) && (mask >> i + 1 & 1) && (mask >> i + 2 & 1)) return 0;
        }
        return 1;
    };
    vi st;
    for (int mask = 0; mask < 1 << n; ++mask) {
        if (check(mask)) {
            st.push_back(mask);
        }
    }
    len = st.size();
    ll ans = 0;
    vector<vvll> dp(m + 1, vvll (1 << n, vll(1 << n)));
    for (int i = 0; i < len; i++) {
        dp[0][st[i]][0] = 1;
    }
    for (int i = 1; i < m; ++i) {
        for (auto& j : st) {
            for (auto& k : st) {
                for (auto& p : st) {
                    if (j & k & p) continue;
                    dp[i][j][k] += dp[i - 1][k][p];
                }
            }
        }
    }
    for (auto& i : st)
        for (auto& j : st) 
            ans += dp[m - 1][i][j];
    print(ans);
    system("pause");
    return 0;
}