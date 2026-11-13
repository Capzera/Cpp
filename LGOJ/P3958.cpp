#include <bits/stdc++.h>
#define int long long
using namespace std;
class UnionFind {
private:
	vector<int> pa;
public:
	UnionFind(int n) {
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
	}
	bool isConnected(int x, int y) {
		return find(x) == find(y);
	}
};

int t, n, h, r, x[1001], y[1001], z[1001];
int dis(int x1, int x2, int y1, int y2, int z1, int z2) {
	return pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2);
}
bool solve() {
	cin >> n >> h >> r;
	UnionFind uf(n);
	vector<int> f1, f2;
	for (int i = 1; i <= n; i++) {
		cin >> x[i] >> y[i] >> z[i];
		if (z[i] >= h - r) f1.push_back(i);
		if (z[i] <= r) f2.push_back(i);
		for (int j = 1; j < i; j++) {
			if (dis(x[i], x[j], y[i], y[j], z[i], z[j]) > 4 * r * r) continue;
			uf.merge(i, j);
		}
	}
	for (auto& u : f1)
		for (auto& v : f2)
			if (uf.isConnected(u, v)) return 1;
	return 0;
}

signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	while (t--) {
		cout << (solve() ? "Yes\n" : "No\n");
	}
	return 0;
}
