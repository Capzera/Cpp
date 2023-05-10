#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, u, v, ans = 0;
    cin >> n >> m;
    vector<int> edge[n + 1];
    for (int i = 0; i < m; i++) {
        cin >> u >> v;
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    function<void(int, int, int)> dfs = [&](int last, int u, int depth) -> void {
        if (depth == 3) {
            ans++;
            return;
        }
        for (auto& v : edge[u]) {
            if (v == last) continue;
            dfs(u, v, depth + 1);
        }
    };
    for (int i = 1; i <= n; i++) {
        dfs(-1, i, 0);
    }
    cout << ans << endl;
    system("pause");
    return 0;
}