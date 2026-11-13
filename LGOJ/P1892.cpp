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
	bool isConnected(int x, int y) {
		return find(x) == find(y);
	}
};
int n, m, x, y, ans;
char op;
int main() {
	cin >> n >> m;
	dsu uf(2 * n);
	while (m--) {
		cin >> op >> x >> y;
		if (op == 'F') uf.merge(x, y);
		else {
			uf.merge(x + n, y);
			uf.merge(y + n, x);
		}
	}
	for (int i = 1; i <= n; i++) ans += uf.find(i) == i;
	cout << ans;
	return 0;
}

