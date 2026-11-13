#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
int n, u, v, a[N], ans[N];
vector<int> g[N];
bool isPrime(int n) {
	if (n < 2) return 0;
	if (n < 4) return 1;
	if (n % 6 != 1 && n % 6 != 5) return 0;
	int m = sqrt(n);
	for (int i = 5; i <= m; i += 6) {
		if (n % i == 0 || n % (i + 2) == 0) return 0;
	}
	return 1;
}
void dfs(int u, int fa, int mx) {
	for (auto& v : g[u]) {
		if (v == fa) continue;
		ans[v] = max(mx, a[u]);
		dfs(v, u, ans[v]);
	}
}
int main() {
	memset(ans, 0xff, sizeof ans);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		scanf("%d", &a[i]);
		if (!isPrime(a[i])) a[i] = -1;
	}
	for (int i = 1; i < n; i++) {
		scanf("%d%d", &u, &v);
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dfs(1, 0, -1);
	for (int i = 1; i <= n; i++) {
		printf("%d ", ans[i]);
	}
	return 0;
}
