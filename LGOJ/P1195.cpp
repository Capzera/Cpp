#include <bits/stdc++.h>
using namespace std;
struct dsu {
	vector<int> pa, size;
	int cnt;
	dsu(int n) {
		pa = size = vector<int>(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
		cnt = n;
	}
	int find(int x) {
		return pa[x] == x ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		if (size[x] < size[y]) swap(x, y);
		pa[y] = x;
		size[x] += size[y];
		cnt--;
	}
	bool isConnected(int x, int y) {
		return find(x) == find(y);
	}
};
struct edge {
	int u, v, w;
	bool operator < (const struct edge& rhs) const {
		return w < rhs.w;
	}
} e[10001];
int n, m, k, ans;
int main() {
	cin >> n >> m >> k;
	for (int i = 1; i <= m; i++) cin >> e[i].u >> e[i].v >> e[i].w;
	sort(e + 1, e + m + 1);
	struct dsu uf(n);
	for (int i = 1; i <= m; i++) {
		if (uf.isConnected(e[i].u, e[i].v)) continue;
		uf.merge(e[i].u, e[i].v);
		ans += e[i].w;
		if (uf.cnt == k) {
			cout << ans;
			return 0;
		}
	}
	cout << -1;
	return 0;
}
