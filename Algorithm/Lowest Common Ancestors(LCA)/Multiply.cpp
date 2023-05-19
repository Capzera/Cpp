#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, r, x, y;
    cin >> n >> m >> r;
    vector<int> edge[n + 1], depth(n + 1), lg(n + 1);
    vector<vector<int>> pa(n + 1, vector<int>(22));
    for(int i = 1; i <= n; i++)
		lg[i] = lg[i - 1] + (1 << lg[i - 1] == i);
    function<void(int, int, int)> dfs = [&](int fa, int sn, int dep) {
        depth[sn] = dep;
        pa[sn][0] = fa;
        for(int i = 1; i <= lg[depth[sn]]; i++) {
            pa[sn][i] = pa[pa[sn][i - 1]][i - 1];
        }
        for(auto& fn : edge[sn]) {
            if (fn == fa) continue;
            dfs(sn, fn, dep + 1);
        }
    };
    auto LCA = [&](int x, int y) -> int {
        if (depth[x] < depth[y]) swap(x, y);
        while(depth[x] > depth[y])
            x = pa[x][lg[depth[x] - depth[y]] - 1];
        if(x == y) return x;
        for(int k = lg[depth[x]] - 1; k >= 0; --k)
            if(pa[x][k] != pa[y][k])
                x = pa[x][k], y = pa[y][k];
        return pa[x][0];
    };
	while (--n) {
        cin >> x >> y;
		edge[x].push_back(y);
        edge[y].push_back(x);
    }
	dfs(0, r, 0);
	while (m--) {
		cin >> x >> y;
		cout << LCA(x, y) << endl;
	}
    system("pause");
	return 0;
}