#include <bits/stdc++.h>
using namespace std;
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int n, k;
    cin >> n >> k;
    vector<vector<vector<long long>>> dp(n + 2, vector<vector<long long>>(k + 1, vector<long long>(1 << n)));
    vector<int> st, cnt(1 << n), trans[1 << n];
    auto check = [&](int mask) -> bool {
        return !(mask & mask >> 1);
    };
    for (int mask = 0; mask < 1 << n; ++mask)
        if (check(mask)) {
            st.push_back(mask);
            cnt[mask] = __builtin_popcount(mask);
        }
    for (auto& i : st)
        for (auto& j : st) {
            if (i & j || !check(i | j)) continue;
            trans[i].push_back(j);
        }
    dp[0][0][0] = 1;
    for (int i = 1; i <= n + 1; ++i)
        for (int j = 0; j <= k; ++j)
            for (auto& l : st) {
                if (j - cnt[l] < 0) continue;
                for (auto& s : trans[l])
                    dp[i][j][l] += dp[i - 1][j - cnt[l]][s];
            }
    cout << dp[n + 1][k][0] << endl;
    system("pause");
    return 0;
}