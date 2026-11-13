#include <bits/stdc++.h>
using namespace std;
const int N = 5e3 + 10, M = 1.4e7 + 10;
int n, x[N], y[N], vis[N];
long long dis[N];
int main() {
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) scanf("%d%d", &x[i], &y[i]);
	memset(dis, 0x3f, sizeof(dis));
	dis[1] = 0;
	for (int i = 1; i <= n; i++) {
		long long u = -1, mn = LLONG_MAX;
		for (int v = 1; v <= n; v++) { // 寻找最小权值点
			if (vis[v] || dis[v] >= mn) continue;
			mn = dis[v];
			u = v;
		}
		vis[u] = 1; // 标记访问
		if (u == n) break; // 提前到达 n 直接结束
		for (int v = 1; v <= n; v++) {
			if (vis[v]) continue;
			long long dist = (x[u] - x[v]) * (x[u] - x[v]) + (y[u] - y[v]) * (y[u] - y[v]);
			dis[v] = min(dis[v], max(dis[u], dist));
		}
	}
	cout << dis[n];
	return 0;
}
