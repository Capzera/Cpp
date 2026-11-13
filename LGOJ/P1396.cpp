#include <bits/stdc++.h>
using namespace std;

class UnionFind {
private:
	vector<int> pa;
	int cnt;
public:
	UnionFind(int n) {
		cnt = n;
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
		cnt--;
	}
	bool isConnected(int x, int y) {
		return find(x) == find(y);
	}
	bool check() {
		return cnt == 1;
	}
};

int n, m, s, t;
struct node {
	int u, v, w;
	bool operator < (const struct node& rhs) {
		return w < rhs.w;
	}
} a[20010];

int main() {
	cin >> n >> m >> s >> t;
	for (int i = 1; i <= m; i++) {
		cin >> a[i].u >> a[i].v >> a[i].w;
	}
	sort(a + 1, a + m + 1);
	UnionFind uf(n);
	for (int i = 1; i <= m; i++) {
		uf.merge(a[i].u, a[i].v);
		if (uf.isConnected(s, t)) {
			cout << a[i].w;
			return 0;
		}
	}
	return 0;
}
