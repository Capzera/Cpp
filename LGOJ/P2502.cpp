#include <bits/stdc++.h>
using namespace std;
struct dsu {
	vector<int> pa, size;
	int cnt;
	dsu(int n) {
		cnt = n;
		pa = size = vector<int>(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
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
struct edge {
	int u, v, w;
	bool operator < (const struct edge& rhs) {
		return w < rhs.w;
	}
} e[5010];
int n, m, u, v, w, s, t, num = -1, den = -1;
double mn = DBL_MAX;
int main() {
	cin >> n >> m;
	struct dsu uf(n);
	for (int i = 1; i <= m; i++) {
		cin >> e[i].u >> e[i].v >>e[i].w;
		uf.merge(e[i].u, e[i].v);
	}
	cin >> s >> t;
	if (!uf.isConnected(s, t)) return puts("IMPOSSIBLE"), 0;
	sort(e + 1, e + m + 1);
	for (int i = 1; i <= m; i++) {
		struct dsu uf(n);
		for (int j = i; j >= 1; j--) {
			int u = e[j].u, v = e[j].v;
			if (uf.isConnected(u, v)) continue;
			uf.merge(u, v);
			if (uf.isConnected(s, t)) {
				if (1.0 * e[i].w < mn * e[j].w) {
					mn = 1.0 * e[i].w / e[j].w;
					num = e[i].w, den = e[j].w;
				}
				break;
			}
		}
	}
	if (num % den == 0) {
		cout << num / den;
		return 0;
	}
	int g = __gcd(num, den);
	cout << num / g << "/" << den / g;
	return 0;
}
