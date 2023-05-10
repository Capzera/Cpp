#include <bits/stdc++.h>
using namespace std;
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    string s, t;
    cin >> s >> t;
    auto split = [&](string s) -> vector<string> {
        int n = s.size();
        vector<string> ans;
        string tmp = "";
        for (auto& ch : s) {
            if (islower(ch)) tmp += ch;
            else {
                if (tmp != "") ans.push_back(tmp);
                tmp = ch;
            }
        }
        if (tmp != "")  ans.push_back(tmp);
        return ans;
    };
    vector<string> word1 = split(s), word2 = split(t);
    int m = word1.size(), n = word2.size();
    vector<vector<int>> dp(m + 1, vector<int> (n + 1));
    for (int i = 1; i <= m; i++) {  
        for (int j = 1; j <= n; j++) {
            if (word1[i - 1] == word2[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    cout << dp[m][n] << endl;
    return 0;
}