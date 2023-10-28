#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, ans = 0;
    char head;
    cin >> n;
    vector<string> words(n);
    vector<int> cnt(n);
    vector<vector<int>> ve(n + 1, vector<int>(n + 1));
    for (int i = 0; i < n; i++) cin >> words[i];
    cin >> head;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int l1 = words[i].size(), l2 = words[j].size(), k = 1;
            for (int k = 1; k < min(l1, l2); k++) {
                if (words[i].substr(l1 - k) == words[j].substr(0, k)) {
                    ve[i][j] = k;
                    break;
                }
            }
        }
    }
    function<void(int, string)> dfs = [&](int last, string s) {
        cnt[last]++;
        ans = max(ans, (int)s.size());
        for (int i = 0; i < n; i++) {
            if (ve[last][i] && cnt[i] < 2) {
                dfs(i, s + words[i].substr(ve[last][i]));
            }
        }
        cnt[last]--;
    };
    for (int i = 0; i < n; i++) {
        if (words[i][0] == head) dfs(i, words[i]);
    }
    cout << ans << endl;
    system("pause");
    return 0;
}