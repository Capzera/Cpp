#include <bits/stdc++.h>
using namespace std;
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    int m, n;
    cin >> m >> n;
    vector<string> mn(m), mx(m);
    unordered_map<string, string> mp;
    string ans = "", put;
    for (int i = 0; i < m; i++) {
        cin >> mn[i];
        sort(mn[i].begin(), mn[i].end());
        mx[i] = mn[i];
        reverse(mx[i].begin(), mx[i].end());
    }
    for (int i = 0; i < m; i++) {
        int flag = 1;
        if (mp.count(mn[i])) {
            ans += mp[mn[i]];
            continue;
        }
        for (int j = 0; flag && j < m; j++) {
            if (i == j || mn[i] <= mx[j]) continue;
            flag = 0;
        }
        mp[mn[i]] = flag ? "1" : "0";
        ans += mp[mn[i]];
    }
    cout << ans << endl;
    system("pause");
    return 0;
}