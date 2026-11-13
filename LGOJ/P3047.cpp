#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10, K = 21;
int n, k, u, v, mx, a[N], dep[N];
long long f1[N][K], f2[N][K];
vector<int> g[N];
void dfs(int u, int fa) {
	f1[u][0] = a[u];
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dfs(v, u);
		for (int i = 1; i <= k; i++) f1[u][i] += f1[v][i - 1];
	}
}
void dfs2(int u, int fa) {
	for (auto& v : g[u]) {
		if (v == fa) continue;
		f2[v][0] = f1[v][0];
		f2[v][1] = f1[v][1] + f2[u][0];
		for (int i = 2; i <= k; i++) {
			f2[v][i] = f1[v][i] + f2[u][i - 1] - f1[v][i - 2];
		}
		dfs2(v, u);
	}
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> k;
	for (int i = 1; i < n; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for (int i = 1; i <= n; i++) cin >> a[i];
	dfs(1, 0);
	for (int i = 0; i <= n; i++) f2[1][i] = f1[1][i];
	dfs2(1, 0);
	for (int i = 1; i <= n; i++) cout << accumulate(f2[i], f2[i] + k + 1, 0ll) << '\n';
	return 0;
}

/*
设 f1[u][i] 表示以u为根的子树，距离恰好为i的奶牛数量
设 f2[u][i] 表示整棵树中距离u 恰好为 i 的奶牛数量

*/
