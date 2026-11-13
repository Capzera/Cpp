#include <bits/stdc++.h>
using namespace std;
struct dsu {
	vector<int> pa;
	int cnt;
	dsu(int n) {
		cnt = n;
		pa = vector<int>(n + 1, 1);
		iota(pa.begin(), pa.end(), 0);
	}
	int find(int x) {
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
};
struct edge {
	int u, v, w;
	bool operator < (const struct edge& rhs) const {
		return w > rhs.w;
	}
} e[100010];
int n, m, u, v, ans;
char op;
int main() {
	cin >> n >> m;
	struct dsu uf(2 * n);
	for (int i = 1; i <= m; i++) cin >> e[i].u >> e[i].v >> e[i].w;
	sort(e + 1, e + m + 1);
	for (int i = 1; i <= m; i++) {
		u = e[i].u, v = e[i].v;
		uf.merge(u, v + n), uf.merge(v, u + n);
		if (uf.isConnected(u, u + n) || uf.isConnected(v, v + n)) {
			cout << e[i].w;
			return 0;
		}
	}
	cout << 0;
	return 0;
}
/*
种类并查集的原理是倍增并查集，将原本1..n的关系改为1..n, n + 1..2n, 2n + 1.. 3n, 3n + 1.. 4n ...
1..n 的关系是朋友-朋友  n+1..2n的关系敌人-敌人  1..n --- n+ 1..2n 关系敌对关系
1 2 3 4
1 和4 是敌人
3 和5是朋友
4 和6是朋友
1 和2 是敌人
朋友           敌人
1 2 3 4 5 6    7 8 9 10 11 12
1 4 3 4 3 4    4 1 9 1  11 12
1, 8
7, 2
1, 2+6
1+6, 2
a, b是敌人
a, c是敌人
uf.merge(a, b + n)
uf.merge(a + n, b)
uf.merge(a, c + n)
uf.merge(a + n, c)
{1}
{2, 4, 6}
{3, 5}
*/

