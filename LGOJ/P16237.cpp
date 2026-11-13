#include <bits/stdc++.h>
using namespace std;
struct dsu {	
public:
	vector<int> pa, sz;
	dsu(int n) : pa(n), sz(n, 1){
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
	}
};
int n, m, u, v, cnt;
int main() {
	cin >> n >> m;
	dsu uf(n + 1);
	while (m--) {
		cin >> u >> v;
		uf.merge(u, v);
	}
	for (int i = 1; i <= n; i++) cnt += uf.find(i) == i;
	cout << max(0, cnt - 1) << " " << (cnt > 1 ? (2 * cnt + n - 3) / n : 0);	
	return 0;
}
