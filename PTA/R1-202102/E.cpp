#include <bits/stdc++.h>
using namespace std;
const int N = 5e4 + 10, M = 2e5 + 10;
struct edge {
	int u, v, w;
	void read() {
		scanf("%d%d%d", &u, &v, &w);
	}
	bool operator < (const struct edge& rhs) const {
		return w < rhs.w;
	}
} e[M];
struct dsu {
	vector<int> pa, sz;
	int n;
	dsu(int _n) {
		n = _n;
		pa = sz = vector<int>(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	int find(int x) {
		return pa[x] == x ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		if (sz[x] < sz[y]) swap(x, y);
		pa[y] = x;
		sz[x] += sz[y];
		n--;
	}
	bool pd(int x, int y) {
		return find(x) == find(y);
	}
};
int n, m, u, v, w, ans = INT_MAX;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= m; i++) e[i].read();
	sort(e + 1, e + m + 1);
	for (int i = 1; i <= m; i++) {
		struct dsu uf(n);
		for (int j = i; j <= m; j++) {
			uf.merge(e[j].u, e[j].v);
			if (uf.n == 1) {
				ans = min(ans, e[j].w - e[i].w);
				break;
			}
		}
	}
	cout << ans;
	return 0;
}
/*

4 6 
1 2 10
2 3 20 
3 4 40 
2 4 80 
1 4 90 
1 3 100 




*/
