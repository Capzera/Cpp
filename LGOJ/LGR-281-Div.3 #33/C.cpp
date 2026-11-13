#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
inline long long read() {
	long long ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans * f;
}
inline void write(long long x) {
	if (x < 0) putchar('-'), x = -x;
	if (x > 9) write(x / 10);
	putchar(x % 10 + '0');
}
long long t, n, d, u, v, a[N], b[N], sz[N], vis[N];
vector<int> g[N];
void dfs(int u, int fa) {
	sz[u] = 1;
	vis[u] = u != d && a[u] >= a[d];
	for (auto& v : g[u]) {
		if (v == fa) continue;
		dfs(v, u);
		if (vis[v]) vis[u] = 1;
		sz[u] += sz[v];
	}
}
long long solve() {
	n = read(), d = read();
	long long ans = 0;
	for (int i = 1; i <= n; i++) g[i].clear();
	for (int i = 1; i <= n; i++) {
		a[i] = read(), b[i] = read();
	}
	for (int i = 1; i < n; i++) {
		u = read(), v = read();
		g[u].push_back(v);
		g[v].push_back(u);
	}
	if (b[d] > 2) return 0ll;
	dfs(d, -1);
	for (auto& v : g[d]) {
		if (!vis[v]) continue;
		ans += sz[v];
	}
	return ans;
}
int main() {
	t = read();
	while (t--) printf("%lld\n", solve());
	return 0;
}
