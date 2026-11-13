#include <bits/stdc++.h>
using namespace std;
const int N = 2100;
long long n, m, d, k, x, ans, mx[N], a[N][N], dp[N][N];
int main() {
	cin >> n >> m >> d;
	for (int j = 1; j <= n; j++) {
		cin >> k;
		while (k--) {
			cin >> x;
			a[x][j]++;
		}
	}
	for (int i = m; i >= 1; i--) {
		for (int j = 1; j <= n; j++) {
			dp[i][j] = dp[i + 1][j] + a[i][j];
			if (i + d <= m) dp[i][j] = max(dp[i][j], mx[i + d] + a[i][j]);
			mx[i] = max(mx[i], dp[i][j]);
		}
	}
	cout << mx[1];
	return 0;
}

