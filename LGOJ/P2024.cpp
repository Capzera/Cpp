#include <bits/stdc++.h>
using namespace std;
struct dsu {
	vector<int> pa;
	dsu(int n) {
		pa = vector<int>(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	int find(int x) {
		return pa[x] == x ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		pa[x] = y;
	}
	bool pd(int x, int y) {
		return find(x) == find(y);
	}
};
int n, m, f, x, y, ans;
int op(dsu& uf) {
	cin >> f >> x >> y;
	if (x > n || y > n) return 1;
	if (f == 2 && x == y) return 1;
	if (f == 1) {
		if (uf.pd(x + n, y) || uf.pd(x, y + n)) return 1;
		uf.merge(x, y);
		uf.merge(x + n, y + n);
		uf.merge(x + 2 * n, y + 2 * n);
	} else {
		if (uf.pd(x, y) || uf.pd(x, y + n)) return 1;
		uf.merge(x + n, y);
		uf.merge(x + 2 * n, y + n);
		uf.merge(x, y + 2 * n);
	}
	return 0;
}
int main() {
	cin >> n >> m;
	dsu uf(3 * n);
	while (m--) ans += op(uf);
	cout << ans;
	return 0;
}

