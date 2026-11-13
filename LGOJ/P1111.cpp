#include <bits/stdc++.h>
using namespace std;
class UnionFind {
private:
	vector<int> pa, size;
	int count;
public:
	UnionFind(int n) {
		count = n;
		pa.resize(n + 1);
		size.resize(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	int find(int x) {
		return pa[x] == x ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		if (size[x] < size[y]) {
			swap(x, y);
		}
		size[x] += y;
		pa[y] = x;
		count--;
	}
	bool check() {
		return count == 1;
	}
};

struct road {
	int x, y, t;
	bool operator < (const struct road& rhs) {
		return t < rhs.t;
	}
} a[100001];
int n, m;
int main() {
	cin >> n >> m;
	UnionFind uf(n);
	for (int i = 1; i <= m; i++) {
		cin >> a[i].x >> a[i].y >> a[i].t;
	}
	sort(a + 1, a + m + 1);
	for (int i = 1; i <= m; i++) {
		uf.merge(a[i].x, a[i].y);
		if (uf.check()) {
			cout << a[i].t;
			return 0;
		}
	}
	cout << -1;
	return 0;
}
