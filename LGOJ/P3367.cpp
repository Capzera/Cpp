#include <bits/stdc++.h>
using namespace std;
class UnionFind {
public:
	vector<int> pa, size;
	int cnt;
	UnionFind(int n) {
		cnt = 0;
		pa = size = vector<int>(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	inline int find(int x) {
		return x == pa[x] ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		if (size[x] < size[y]) swap(x, y);
		size[x] += y;
		pa[y] = x;
		cnt++;
	}
	bool isCon(int x, int y) {
		return find(x) == find(y);
	}
};

int n, m, u, v, op;
int main() {
	cin >> n >> m;
	UnionFind uf(n);
	while (m--) {
		cin >> op >> u >> v;
		if (op == 1) uf.merge(u, v);
		else cout << (uf.isCon(u, v) ? "Y\n" : "N\n");
	}
	return 0;
}
