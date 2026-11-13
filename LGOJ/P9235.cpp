#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 1e5 + 10;
struct node {
	int u, v, w;
	void read() {
		scanf("%d%d%d", &u, &v, &w);
	}
	bool operator < (const struct node& rhs) const {
		return w > rhs.w;
	}
} e[N * 3];
vector<pii> g[N];
vector<int> pa, sz;
int find(int x) {
	return pa[x] == x ? x : pa[x] = find(pa[x]);
}
int n, m, q, u, v, w, ans[N];
int main() {
	scanf("%d%d%d", &n, &m, &q);
	memset(ans, -1, sizeof(ans));
	pa = sz = vector<int>(n + 1, 1);
	iota(pa.begin(), pa.end(), 0);
	for (int i = 1; i <= m; i++) e[i].read();
	sort(e + 1, e + m + 1);
	for (int i = 1; i <= q; i++) {
		scanf("%d%d", &u, &v);
		g[u].push_back({v, i});
		g[v].push_back({u, i});
	}
	for (int i = 1; i <= m; i++) {
		int u = e[i].u, v = e[i].v;
		u = find(u), v = find(v);
		if (u == v) continue;
		if (sz[u] < sz[v]) swap(u, v);
		for (auto& p : g[v]) {
			int fn = p.first, id = p.second;
			if (find(fn) == u) {
				ans[id] = e[i].w;
			} else {
				g[u].push_back(p);
			}
		}
		sz[u] += sz[v];
		pa[v] = u;
	}
	for (int i = 1; i <= q; i++) {
		printf("%d\n", ans[i]);
	}
	return 0;
}
/*
时间复杂度O(m logm + mα(n) + q logn α(n)) = O(m logm + q logn)
空间复杂度O(m + q + n)
u = 较大树的根, v = 较小数的根

遍历包含较小数的根的孩子(带有较小数的根的查询数对)
如果该查询数对已经被拿进较大数(连通)，对答案设置
否则该连通情况被作为较大数的根的孩子
	因为较小数根即将被合并进较大数，所以对孩子进行转移
	原本孩子属于v--->孩子属于u

u, v, w
对于所有查询x∈U，y∈V的查询均有答案


Kruskal重构树

5 4 3

2 3 6
1 2 5
1 4 3
3 4 1

{1, 2, 3}
1, 4   2, 4   3,4 


1 5
2 4
1 3

连通块{1, 2, 3, 4}
1 2 3 4 5
2 2 2 4 5
1 <- 4
把4合并到1上
遍历4的孩子
g[2]:{{4, 2}, {1, 3}, {5, 1}}
g[5]:{}
ans 1  2  3
   -1  3  5

*/
