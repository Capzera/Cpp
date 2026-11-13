#include <bits/stdc++.h>
using namespace std;
const int N = 501, dx[] = {0, 1}, dy[] = {1, 0};
struct node {
	long long u, v, w;
	bool operator < (const struct node& rhs) const {
		return w < rhs.w;
	}
};
long long n, m, a[N][N];
struct dsu {
	vector<int> pa, size;
	dsu(int _n) {
		pa = size = vector<int>(_n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	int find(int x) {
		return x == pa[x] ? x : pa[x] = find(pa[x]);
	}
	int merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return 0;
		pa[y] = x;
		size[x] += size[y];
		if (size[x] >= m) return 1;
		return 2;
	}
};
int main() {
	scanf("%lld", &n);
	m = (n * n + 1) >> 1;
	vector<node> g;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%lld", &a[i][j]);
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int d = 0; d < 2; d++) {
				int x = i + dx[d], y = j + dy[d];
				if (x < 0 || x >= n || y < 0 || y >= n) continue;
				g.push_back({i * n + j, x * n + y, abs(a[i][j] - a[x][y])});
			}
		}
	}
	sort(g.begin(), g.end());
	struct dsu uf(n * n);
	for (auto& p : g) {
		if (uf.merge(p.u, p.v) != 1) continue;
		cout << p.w;
		break;
	}
	return 0;
}
