#include <bits/stdc++.h>
using namespace std;
int t, n, vis[11];
struct plane {
	int t, d, l;
} a[11];

bool dfs(int i, int sum) {
	if (i > n) return 1;
	for (int j = 1; j <= n; j++) {
		if (vis[j]) continue;
		if (a[j].t + a[j].d < sum) continue;
		vis[j] = 1;
		if (dfs(i + 1, max(sum, a[j].t) + a[j].l)) return 1;
		vis[j] = 0;
	}
	return 0;
}

bool solve() {
	memset(vis, 0, sizeof(vis));
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].t >> a[i].d >> a[i].l;
	}
	return dfs(1, 0);
}
int main() {
	cin >> t;
	while (t--) {
		cout << (solve() ? "YES\n" : "NO\n");
	}
	return 0;
}
/*
0 2 20
10 10 10
0 100 10
*/
