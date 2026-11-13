#include <bits/stdc++.h>
using namespace std;
const int N = 505, M = 2e4 + 5;
struct node { int u, v, w;} e[M];
int n, m;
long long ans = LLONG_MAX, dp[N][N];
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	memset(dp, 0x3f, sizeof dp);
	cin >> n >> m;
	if (n < 4) return puts("-1"), 0;
	for (int i = 1; i <= m; i++) {
		cin >> e[i].u >> e[i].v >> e[i].w;
	}
	dp[0][1] = 0;
	for (int i = 1; i < n >> 1; i++) {
		for (int j = 1; j <= m; j++) {
			int u = e[j].u, v = e[j].v, w = e[j].w;
			dp[i][v] = min(dp[i][v], dp[i - 1][u] + w);
		}
		ans = min(ans, dp[i][n]);
	}
	if (ans > 1e18) ans = -1;
	cout << ans;
	return 0;
}
