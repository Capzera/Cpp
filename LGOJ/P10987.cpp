#include<bits/stdc++.h>
#define int long long
using namespace std;
const int N = 210, M = 1e3 + 10;
int n, a, b, ans, w[N], dp[M][M];
signed main() {
	cin >> n >> a >> b;
	for (int i = 1; i <= n; i++) cin >> w[i];
	for (int i = 1; i <= n; i++)
		for (int j = a; j >= 0; j--)
			for (int k = b; k >= 0; k--) {
				if (j >= w[i]) dp[j][k] = max(dp[j][k], dp[j - w[i]][k] + w[i]);
				if (k >= w[i]) dp[j][k] = max(dp[j][k], dp[j][k - w[i]] + w[i]);
				ans = max(ans, dp[j][k]);
			}
	cout << ans;
	return 0;
}

