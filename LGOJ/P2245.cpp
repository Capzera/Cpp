#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 1e5 + 1;
struct edge {
	int u, v, w;
	void read() {
		scanf("%d%d%d", &u, &v, &w);
	}
	bool operator < (const struct edge& rhs) const {
		return w < rhs.w;
	}
} e[N * 3];
int n, m, q, u, v, ans[N];
vector<int> pa, sz;
vector<pii> g[N];
int find(int x) {
	return x == pa[x] ? x : pa[x] = find(pa[x]);
}
int main() {
	memset(ans, -1, sizeof(ans));
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= m; i++) e[i].read();
	sort(e + 1, e + m + 1);
	pa = sz = vector<int> (n + 1, 1);
	iota(pa.begin(), pa.end(), 0);
	scanf("%d", &q);
	for (int i = 1; i <= q; i++) {
		scanf("%d%d", &u, &v);
		g[u].push_back({v, i});
		g[v].push_back({u, i});
	}
	for (int i = 1; i <= m; i++) {
		u = e[i].u, v = e[i].v;
		u = find(u), v = find(v);
		if (u == v) continue;
		if (sz[u] < sz[v]) swap(u, v);
		for (auto& p : g[v]) {
			int sn = p.first, id = p.second;
			if (find(sn) == u) {
				ans[id] = e[i].w;
			} else {
				g[u].push_back(p);
			}
		}
		sz[u] += sz[v];
		pa[v] = u;
	}
	for (int i = 1; i <= q; i++) {
		if (ans[i] == -1) puts("impossible");
		else printf("%d\n", ans[i]);
	}
	return 0;
}
