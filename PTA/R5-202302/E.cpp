#include <bits/stdc++.h>
using namespace std;
const int N = 90;
int n, m;
long long ans;
long long a[N], dp[N][N], base[N];
int main() {
	cin >> m >> n;
	base[0] = 1;
	for (int i = 1; i <= n; i++) base[i] = base[i - 1] << 1;
	while (m--) {
		for (int i = 1; i <= n; i++) cin >> a[i];
		memset(dp, 0, sizeof(dp));
		long long mx = 0;
		for (int len = n; len; len--) {
			for (int i = 1; i <= n - len + 1; i++) {
				int j = i + len - 1;
				dp[i][j] = max(dp[i - 1][j] + a[i - 1] * base[n - j + i - 1], dp[i][j + 1] + a[j + 1] * base[n - j + i - 1]);
			}
		}
		for (int i = 1; i <= n; i++) {
			mx = max(mx, dp[i][i] + base[n] * a[i]);
		}
		ans = ans + mx;
	}
	cout << ans;
	return 0;
}

