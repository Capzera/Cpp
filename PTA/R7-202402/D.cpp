#include <bits/stdc++.h>
using namespace std;
const int N = 10, dx[] = {0, 1}, dy[] = {1, 0};
int n, u, v, w, ans, mx, a[N][N], dp[N][N][N][N];
int main() {
	cin >> n;
	while (cin >> u >> v >> w, u && v && w) {
		a[u][v] = w;
	}
	for (int x1 = 1; x1 <= n; x1++) {
		for (int y1 = 1; y1 <= n; y1++) {
			for (int x2 = 1; x2 <= n; x2++) {
				for (int y2 = 1; y2 <= n; y2++) {
					dp[x1][y1][x2][y2] = max({dp[x1 - 1][y1][x2 - 1][y2], dp[x1 - 1][y1][x2][y2 - 1], dp[x1][y1 - 1][x2 - 1][y2], dp[x1][y1 - 1][x2][y2 - 1]}) + a[x1][y1] + a[x2][y2];
					if (x1 == x2 && y1 == y2) dp[x1][y1][x2][y2] -= a[x1][y1];
				}
			}
		}
	}
	cout << dp[n][n][n][n];
	return 0;
}

