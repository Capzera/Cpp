/*
给出一个长度不超过 200 的由小写英文字母组成的字母串（该字串以每行 20 个字母的方式输入，
且保证每行一定为 20 个）。要求将此字母串分成 kk 份，且每份中包含的单词个数加起来总数最大。
每份中包含的单词可以部分重叠。当选用一个单词之后，其第一个字母不能再用。
例如字符串 this 中可包含 this 和 is，选用 this 之后就不能包含 th。
单词在给出的一个不超过 6 个单词的字典中。
要求输出最大的个数。
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
    int p, k, m, n;
    cin >> p >> k;
    string str = "", tmp;
    for (int i = 0; i < p; i++) cin >> tmp, str += tmp;
    n = 20 * p;
    cin >> m;
    unordered_set<string> words;
    vector<vector<int> > sum(n + 1, vector<int>(n + 1)), dp(n + 1, vector<int>(k + 1));
    for (int i = 0; i < m; i++) {
        cin >> tmp;
        words.emplace(tmp);
    }

    for (int i = 0; i < n; i++) {
        tmp = "";
        for (int j = i; j < n; j++) {
            tmp += str[j];
            sum[i + 1][j + 1] = sum[i][j] + words.count(tmp);
        }
    }

    for (int i = 1; i <= k; i++) dp[i][i] = dp[i - 1][i - 1] + sum[i][i];
    for (int i = 1; i <= n; i++) dp[i][1] = sum[1][i];

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= min(i, k); j++) {
            for (int l = j; l < i; l++) {
                dp[i][j] = max(dp[i][j], dp[l][j - 1] + sum[l + 1][j]);
            }
        }
    }
    for (auto& i : dp) {
        for (auto& j : i) cout << j << " ";
        cout << endl;
    }
    cout << dp[n][k] << endl;
    system("pause");
    return 0;
}
/*
dp[i][j] 前i个字母分成j个的最大结果
thisisabookyouareaoh
is
a
ok
sabz

dp[i][]
*/