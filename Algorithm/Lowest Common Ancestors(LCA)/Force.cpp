#include <bits/stdc++.h>
using namespace std;
int n, m, r, x, y;
int main() {
    cin >> n >> m >> r;
    vector<int> depth(n + 1), pa(n + 1), edge[n + 1];
    while (--n) {
        cin >> x >> y;
        edge[x].push_back(y);
        edge[y].push_back(x);
    }
    function<void(int, int, int)> dfs = [&](int fa, int sn, int dep) -> void {
        depth[sn] = dep;
        pa[sn] = fa;
        for (auto& fn : edge[sn]) {
            if (fn == fa) continue;
            dfs(sn, fn, dep + 1);
        }
    };
    auto LCA = [&](int x, int y) -> int {
        while (x != y) {
            if (depth[x] >= depth[y]) x = pa[x];
            else y = pa[y];
        }
        return x;
    };
    dfs(0, r, 0);
    while (m--) {
        cin >> x >> y;
        cout << LCA(x, y) << endl;
    }
    system("pause");
    return 0;
}