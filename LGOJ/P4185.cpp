#include <bits/stdc++.h>
using namespace std;
class UnionFind {	
public:
	vector<int> pa, size;
	UnionFind(int n) {
		pa = size = vector<int>(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	inline int find(int x) {
		return pa[x] == x ? x : pa[x] = find(pa[x]);
	}
	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		if (size[x] < size[y]) swap(x, y);
		size[x] += size[y];
		pa[y] = x;
	}
	int query(int x) {
		return size[find(x)] - 1;
	}
};
struct node {
	int u, v, w;
	bool operator < (const struct node& rhs) {
		return w > rhs.w;
	}
};
int n, q, k, u, v, w, ans[100001];
int main() {
	cin >> n >> q;
	UnionFind uf(n);
	vector<node> a(n - 1), que(q);
	vector<int> ans(q);
	for (auto& x : a) {
		cin >> x.u >> x.v >> x.w;
	}
	for (int i = 0; i < q; i++) {
		cin >> que[i].w >> que[i].v;
		que[i].u = i;
	}
	sort(a.begin(), a.end());
	sort(que.begin(), que.end());
	for (int i = 0, j = 0; i < q; i++) {
		while (j < n - 1 && a[j].w >= que[i].w) {
			uf.merge(a[j].u, a[j].v);
			j++;
		}
		ans[que[i].u] = uf.query(que[i].v);
	}
	for (auto& x : ans) cout << x << endl;
	return 0;
}
