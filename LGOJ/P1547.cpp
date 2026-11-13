#include <bits/stdc++.h>
using namespace std;

class UnionFind {	
public:
	vector<int> pa;
	int cnt;
	UnionFind(int n) {
		cnt = 0;
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
		cnt++;
	}
};

struct node {
	int u, v, w;
	bool operator < (const struct node& rhs) {
		return w < rhs.w;
	}
} a[10001];
int n, m;
int main() {
	cin >> n >> m;
	UnionFind uf(n);
	for (int i = 1; i <= m; i++) cin >> a[i].u >> a[i].v >> a[i].w;
	sort(a + 1, a + m + 1);
	for (int i = 1; i <= m; i++) {
		uf.merge(a[i].u, a[i].v);
		if (uf.cnt == n - 1) {
			cout << a[i].w;
			return 0;
		}
	}
	cout << -1;
	return 0;
}
