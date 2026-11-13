#include <bits/stdc++.h>
using namespace std;

class UnionFind {	
public:
	vector<int> pa;
	int cnt;
	UnionFind(int n) {
		cnt = n;
		pa = vector<int>(n + 1);
		iota(pa.begin(), pa.end(), 0);
	}
	inline int find(int x) {
		return pa[x] == x ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		pa[y] = x;
		cnt--;
	}
	bool isConnected(int x, int y) {
		return find(x) == find(y);
	}
	int check() {
		return cnt;
	}
};

int n, m, u, v;
int vis[200010], node[200010], ans[200010];
vector<int> g[200010];

int main() {
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	UnionFind uf(n);
	for (int i = 1; i <= n; i++) cin >> node[i];
	ans[n] = 1; vis[node[n]] = 1;
	for (int i = n - 1; i > 0; i--) {
		int u = node[i];
		vis[u] = 1;
		for (auto& v : g[u]) {
			if (!vis[v]) continue;
			uf.merge(u, v);
		}
		ans[i] = uf.cnt == i;
	}
	for (int i = 1; i <= n; i++) {
		cout << (ans[i] ? "YES\n" : "NO\n");
	}
	return 0;
}
