#include <bits/stdc++.h>
using namespace std;
int solve() {
    string s1, s2;
    cin >> s1 >> s2;
    int n = 9, point = s1.find('.');
    int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
    unordered_map<string, int> mp;
    queue<pair<string, int>> q;
    mp[s1] = 0, q.push({s1, point});
    while (q.size()) {
        auto [grid, pos] = q.front(); q.pop();
        if (grid == s2) return mp[grid];
        int x = pos / 3, y = pos % 3;
        for (int d = 0; d < 4; d++) {
            int mx = x + dx[d], my = y + dy[d], nPos = mx * 3 + my;
            if  (mx < 0 || mx == 3 || my < 0 || my == 3) continue;
            string str = grid;
            swap(str[pos], str[nPos]);
            if (mp.count(str)) continue;
            mp[str] = mp[grid] + 1;
            q.push({str, nPos});
        }
    }
    return -1;
}
int main() {
    cout << solve() << endl;
    system("pause");
    return 0;
}
/*
123
4.5
678

*/