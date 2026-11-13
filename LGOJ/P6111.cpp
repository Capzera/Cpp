#include <bits/stdc++.h>
using namespace std;
int n, q, k, u, v, w, vis[100001];
class UnionFind {	
public:
	vector<int> pa;
	int cnt;
	void init(int n) {
		cnt = 0;
		pa = vector<int>(n + 1);
		iota(pa.begin(), pa.end(), 0);
	}
	UnionFind() {
		init(n);
	}
	inline int find(int x) {
		return pa[x] == x ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		pa[y] = x;
		cnt++;
	}
	bool isConnected(int x, int y) {
		return find(x) == find(y);
	}
	int check() {
		return cnt;
	}
};
UnionFind uf;
vector<pair<int, int> > g[100001];
void dfs(int u) {
	vis[u] = 1;
	for (auto& fn : g[u]) {
		v = fn.first, w = fn.second;
		if (vis[v] || w < k) continue;
		uf.merge(u, v);
		dfs(v);
	}
}
int main() {
	cin >> n >> q;
	
	for (int i = 1; i < n; i++) {
		cin >> u >> v >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, w});
	}
	while (q--) {
		cin >> k >> u;
		memset(vis, 0, sizeof(vis));
		uf.init(n);
		dfs(u);
		cout << uf.check() << endl;
	}
	return 0;
}
