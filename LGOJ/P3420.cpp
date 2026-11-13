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
int n, x;
int main() {
	cin >> n;
	struct dsu uf(n);
	for (int i = 1; i <= n; i++) {
		cin >> x;
		uf.merge(i, x);
	}
	cout << uf.cnt;
	return 0;
}
