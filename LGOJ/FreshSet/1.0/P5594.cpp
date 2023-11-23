#include <bits/stdc++.h>
using namespace std;
int main() {
    int m, n, k, put;
    cin >> m >> n >> k;
    vector<set<int>> ans(k);
    while (m--) 
        for (int i = 0; i < n; i++) {
            cin >> put;
            ans[put - 1].emplace(i);
        }
    for (auto& x : ans) cout << x.size() << " ";
    system("pause");
    return 0;
}