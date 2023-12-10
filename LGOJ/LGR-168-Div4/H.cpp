#include <bits/stdc++.h>
using namespace std;
int main() {
    int m, n, p;
    cin >> m;
    vector<vector<int> > grid(m);
    vector<int> score(m);
    unordered_map<int, long long> mp;
    long long ans, mx = 0;
    for (int i = 0; i < m; i++) {
        cin >> p;
        grid[i].resize(p);
        for (auto& x : grid[i]) cin >> x;
        cin >> score[i];
    }
    cin >> n;
    vector<int> ret(n);
    for (auto& x : ret) cin >> x;

    for (int i = 0; i < m; i++) {
        int ch = ret[grid[i][0] - 1], flag = 1;
        for (int j = 1; flag && j < grid[i].size(); j++) {
            if (ret[grid[i][j] - 1] != ch) flag = 0;
        }
        if (flag) mp[ch] += score[i];
    }

    for (auto& [x, time] : mp) {
        if (time > mx) {
            mx = time;
            ans = x;
        }
    }
    cout << mx << endl << ans << endl;
    system("pause");
    return 0;
}