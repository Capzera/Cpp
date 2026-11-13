#include <bits/stdc++.h>
using namespace std;
class UnionFind {	
public:
	vector<int> pa, size;
	UnionFind(int n) {
		pa = size = vector<int>(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	inline int find(int x) {
		return pa[x] == x ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		if (size[x] < size[y]) swap(x, y);
		size[x] += size[y];
		pa[y] = x;
	}
	int query() {
		return size[find(1)];
	}
};

int n, m, p, q, u, v;
int main() {
	cin >> n >> m >> p >> q;
	UnionFind uf1(n), uf2(m);
	while (p--) {
		cin >> u >> v;
		uf1.merge(u, v);
	}
	while (q--) {
		cin >> u >> v;
		uf2.merge(-u, -v);
	}
	cout << min(uf1.query(), uf2.query());
	return 0;
}
