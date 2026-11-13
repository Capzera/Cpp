#include <bits/stdc++.h>
using namespace std;
struct dsu {
	vector<int> pa, size;
	int cnt;
	dsu(int n) {
		pa = size = vector<int> (n + 1, 1);
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
	bool pd(int x, int y) {
		return find(x) == find(y);
	}
};
struct edge{
	int u, v, w;
	bool operator < (const struct edge& rhs) {
		return w < rhs.w;
	}
	void read() {
		cin >> u >> v >> w;
	}
} a[200010];
int n, m;
int main() {
	cin >> n >> m;
	struct dsu uf(n);
	for (int i = 1; i <= m; i++) a[i].read();
	sort(a + 1, a + m + 1);
	for (int i = 1; i <= m; i++) {
		if (uf.pd(a[i].u, a[i].v)) continue;
		uf.merge(a[i].u, a[i].v);
		if (uf.cnt == 1) {
			cout << a[i].w;
			return 0;
		}
	}
	cout << -1;
	return 0;
}
